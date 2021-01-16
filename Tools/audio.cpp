/*
		Project:		MIDI Tools
		Module:			audio.cpp
		Description:	The synthesizer helper module with audio stream
		Author:			Martin Gäckler
		Address:		Hopfengasse 15. A-4020 Linz
		Web:			https://www.gaeckler.at/

		Copyright:		(c) 2005-2018 Martin Gäckler

		This program is free software: you can redistribute it and/or modify  
		it under the terms of the GNU General Public License as published by  
		the Free Software Foundation, version 3.

		You should have received a copy of the GNU General Public License 
		along with this program. If not, see <http://www.gnu.org/licenses/>.

		THIS SOFTWARE IS PROVIDED BY Martin Gäckler, Germany, Munich ``AS IS''
		AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
		TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
		PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
		CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
		SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
		LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
		USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
		ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
		OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
		OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
		SUCH DAMAGE.
*/


// --------------------------------------------------------------------- //
// ----- switches ------------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- includes ------------------------------------------------------ //
// --------------------------------------------------------------------- //

#include <Audioclient.h>
#include <Mmdeviceapi.h>

#include <gak/thread.h>
#include <gak/fmtNumber.h>
#include <gak/directory.h>
#include <gak/wavefile.h>

#include <WINLIB/POPUP.H>
#include <WINLIB/DEVICE.H>

#include "statusWindow.h"
#include "audio.h"
#include "midifile.h"

// --------------------------------------------------------------------- //
// ----- imported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module switches ----------------------------------------------- //
// --------------------------------------------------------------------- //

#ifdef __BORLANDC__
#	pragma option -RT-
#	pragma option -b
#	pragma option -a4
#	pragma option -pc
#endif

using namespace gak;
using namespace winlib;

// --------------------------------------------------------------------- //
// ----- constants ----------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- macros -------------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- type definitions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class definitions --------------------------------------------- //
// --------------------------------------------------------------------- //

class AVRTtask
{
	typedef HANDLE (WINAPI *AvSetMmThreadCharacteristicsPtr)( const char *TaskName, LPDWORD TaskIndex );
	typedef BOOL (WINAPI *AvRevertMmThreadCharacteristicsPtr)( HANDLE AvrtHandle );

	static HMODULE								s_avrtDLL;

	static AvSetMmThreadCharacteristicsPtr		s_AvSetMmThreadCharacteristicsFunc;
	static AvRevertMmThreadCharacteristicsPtr	s_AvRevertMmThreadCharacteristicsFunc;

	HANDLE										m_taskHandle;

	public:
	AVRTtask()
	{
		if( !s_avrtDLL )
		{
			s_avrtDLL = LoadLibrary( "AVRT.DLL" );
		}

		if( s_avrtDLL && !s_AvSetMmThreadCharacteristicsFunc )
		{
			s_AvSetMmThreadCharacteristicsFunc = reinterpret_cast<AvSetMmThreadCharacteristicsPtr>(
				GetProcAddress(s_avrtDLL, "AvSetMmThreadCharacteristicsA" ) 
			);
		}

		if( s_AvSetMmThreadCharacteristicsFunc )
		{
			DWORD	taskIndex = 0;
			m_taskHandle = s_AvSetMmThreadCharacteristicsFunc( "Pro Audio", &taskIndex );
		}
		else
		{
			m_taskHandle = NULL;
		}
	}
	~AVRTtask()
	{
		if( m_taskHandle )
		{
			if( !s_avrtDLL )
			{
				s_avrtDLL = LoadLibrary( "AVRT.DLL" );
			}

			if( s_avrtDLL && !s_AvRevertMmThreadCharacteristicsFunc )
			{
				s_AvRevertMmThreadCharacteristicsFunc = reinterpret_cast<AvRevertMmThreadCharacteristicsPtr>(
					GetProcAddress(s_avrtDLL, "AvRevertMmThreadCharacteristics" )
				);
			}
			if( s_AvRevertMmThreadCharacteristicsFunc )
			{
				s_AvRevertMmThreadCharacteristicsFunc( m_taskHandle );
			}
		}
	}
	HANDLE get( void ) const
	{
		return m_taskHandle;
	}

};

/*
	the audio thread
*/
class AudioThread : public Thread
{
	public:
	static RingBuffer< StereoSample<> >	s_meterBuffer;
	static MidiSynthesizer<>			s_synthesizer;

	private:
    WAVEFORMATEX	m_pwfx;
	double			m_latency;
	enum AudioErrors
	{
		InitFailed, NoDefaultEndPoint, NoAudioClient, WaveFormatNotSupport, NoDevicePeriod, NoBufferSize, ClientInitFailed, EventFailed, 
		NoRenderClient, NoBuffer, NoData, NoTask, StartFailed, AudioTimeout
	};
	struct AudioException
	{
		AudioErrors	errCode;
		HRESULT		windowsError;

		AudioException( AudioErrors newErrCode, HRESULT	newWinError ) : errCode( newErrCode ), windowsError( newWinError ) {}
	};

	void PlayAudioStream( void );
	virtual void ExecuteThread( void );

	public:
	AudioThread( unsigned short bitsPerSample, unsigned long sampleRate );
	void setLatency( double newLatency )
	{
		m_latency = newLatency;
	}
	double getLatency( void ) const
	{
		return m_latency;
	}

    void CheckMixFormat( IAudioClient *pAudioClient )
	{
		WAVEFORMATEX *match;

		HRESULT hr = pAudioClient->IsFormatSupported( AUDCLNT_SHAREMODE_EXCLUSIVE, &m_pwfx, &match );
		if( FAILED( hr ) )
		{
			throw AudioException( WaveFormatNotSupport, hr );
		}
	}
	HRESULT LoadData( UINT32 frameCount, BYTE *data, DWORD *flags );
};


/*
	level meter thread
*/
class LevelMeterThread : public Thread
{
	ToolWindow						*m_targetWindow;
	RingBuffer< StereoSample<> >	&m_dataSource;

	virtual void ExecuteThread( void );
	public:
	LevelMeterThread( RingBuffer< StereoSample<> > &dataSource, ToolWindow *targetWindow ) : m_dataSource( dataSource )
	{
		m_targetWindow = targetWindow;
	}
};

// --------------------------------------------------------------------- //
// ----- exported datas ------------------------------------------------ //
// --------------------------------------------------------------------- //

RingBuffer< StereoSample<> >	AudioThread::s_meterBuffer( sampleRate );
MidiSynthesizer<>				AudioThread::s_synthesizer;

// --------------------------------------------------------------------- //
// ----- module static data -------------------------------------------- //
// --------------------------------------------------------------------- //

static SharedObjectPointer<AudioThread>			theAudioThread;

static SharedObjectPointer<LevelMeterThread>	theMeterThread;
static ToolWindow								*meterWindow;

static SharedObjectPointer<StatusThread<> >		theStatusThread;
static StatusWindow<>							*statusWindow;

static size_t									openCount = 0;

// --------------------------------------------------------------------- //
// ----- class static data --------------------------------------------- //
// --------------------------------------------------------------------- //

HMODULE AVRTtask::s_avrtDLL = 0;

AVRTtask::AvSetMmThreadCharacteristicsPtr		AVRTtask::s_AvSetMmThreadCharacteristicsFunc = 0;
AVRTtask::AvRevertMmThreadCharacteristicsPtr	AVRTtask::s_AvRevertMmThreadCharacteristicsFunc = 0;

// --------------------------------------------------------------------- //
// ----- prototypes ---------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- module functions ---------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class inlines ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class constructors/destructors -------------------------------- //
// --------------------------------------------------------------------- //

AudioThread::AudioThread( unsigned short bitsPerSample, unsigned long sampleRate )
{
	m_pwfx.wFormatTag = WAVE_FORMAT_PCM;
	m_pwfx.nChannels = 2;
	m_pwfx.nSamplesPerSec = sampleRate;
	m_pwfx.wBitsPerSample = bitsPerSample;
	m_pwfx.cbSize = 0;

	m_pwfx.nBlockAlign = m_pwfx.nChannels * m_pwfx.wBitsPerSample / 8;
	m_pwfx.nAvgBytesPerSec = m_pwfx.nSamplesPerSec * m_pwfx.nBlockAlign;
	m_latency = 0;
}

// --------------------------------------------------------------------- //
// ----- class static functions ---------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class privates ------------------------------------------------ //
// --------------------------------------------------------------------- //

HRESULT AudioThread::LoadData( UINT32 frameCount, BYTE *data, DWORD *flags )
{
	if( terminated )
	{
		*flags = AUDCLNT_BUFFERFLAGS_SILENT;
		return 0;
	}

	StereoSample<>	mySample;

	*flags = 0;
	if( m_pwfx.wBitsPerSample == 32 )
	{
		int		*samplePtr = (int *)data;

		while( frameCount-- )
		{
			s_synthesizer.getNextSample( mySample );
			s_meterBuffer.push( mySample );

			mySample.left <<= 8;
			mySample.right <<= 8;

			*samplePtr++ = mySample.left;
			*samplePtr++ = mySample.right;
		}
	}
	else if( m_pwfx.wBitsPerSample == 24 )
	{
		char	*leftSamplePtr = (char *)&mySample.left;
		char	*rightSamplePtr = (char *)&mySample.right;
		while( frameCount-- )
		{
			s_synthesizer.getNextSample( mySample );
			s_meterBuffer.push( mySample );

			*data++ = leftSamplePtr[0];
			*data++ = leftSamplePtr[1];
			*data++ = leftSamplePtr[2];

			*data++ = rightSamplePtr[0];
			*data++ = rightSamplePtr[1];
			*data++ = rightSamplePtr[2];

		}
	}
	else if( m_pwfx.wBitsPerSample == 16 )
	{
		short	*samplePtr = (short *)data;
		while( frameCount-- )
		{
			s_synthesizer.getNextSample( mySample );
			s_meterBuffer.push( mySample );

			mySample.left >>= 8;
			mySample.right >>= 8;
			*samplePtr++ = short(mySample.left);
			*samplePtr++ = short(mySample.right);
		}
	}
	else
	{
		throw AudioException( WaveFormatNotSupport, E_FAIL );
	}

	return NOERROR;
}

//-----------------------------------------------------------
// Play an audio stream on the default audio rendering
// device. The PlayAudioStream function allocates a shared
// buffer big enough to hold one second of PCM audio data.
// The function uses this buffer to stream data to the
// rendering device. The inner loop runs every 1/2 second.
//-----------------------------------------------------------

// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

void AudioThread::PlayAudioStream(  void )
{
	HRESULT											hr;
	REFERENCE_TIME									hnsRequestedDuration = 0;
	gak::windows::ComPointer<IMMDeviceEnumerator>	pEnumerator;
	gak::windows::ComPointer<IMMDevice>				pDevice;
	gak::windows::ComPointer<IAudioClient>			pAudioClient;
	gak::windows::ComPointer<IAudioRenderClient>	pRenderClient;
	gak::windows::SystemEvent						hEvent;

	UINT32											bufferFrameCount;
	BYTE											*pData;
	DWORD											flags = 0;

	hr = CoInitialize( NULL );
    if( FAILED( hr ) )
	{
		throw AudioException( InitFailed, hr );
	}

    hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&pEnumerator
	);
    if( FAILED( hr ) )
	{
		throw AudioException( InitFailed, hr );
	}

    hr = pEnumerator->GetDefaultAudioEndpoint( eRender, eConsole, &pDevice );
    if( FAILED( hr ) )
	{
		throw AudioException( NoDefaultEndPoint, hr );
	}

    hr = pDevice->Activate(
		IID_IAudioClient, CLSCTX_ALL,
		NULL, (void**)&pAudioClient
	);
    if( FAILED( hr ) )
	{
		throw AudioException( NoAudioClient, hr );
	}

	CheckMixFormat( pAudioClient );

	// Initialize the stream to play at the minimum latency.
	hr = pAudioClient->GetDevicePeriod( NULL, &hnsRequestedDuration );
    if( FAILED( hr ) )
	{
		throw AudioException( NoDevicePeriod, hr );
	}

    hr = pAudioClient->Initialize(
		AUDCLNT_SHAREMODE_EXCLUSIVE,
		AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
		hnsRequestedDuration,
		hnsRequestedDuration,
		&m_pwfx,
		NULL
	);
	if( hr == AUDCLNT_E_BUFFER_SIZE_NOT_ALIGNED )
	{
		hr = pAudioClient->GetBufferSize(&bufferFrameCount);
	    if( FAILED( hr ) )
		{
			throw AudioException( NoBufferSize, hr );
		}

		hnsRequestedDuration = (REFERENCE_TIME)
		((10000.0 * 1000 / m_pwfx.nSamplesPerSec * bufferFrameCount) + 0.5);

		pAudioClient.Release();

		hr = pDevice->Activate(
			IID_IAudioClient, CLSCTX_ALL,
			NULL, (void**)&pAudioClient
		);
	    if( FAILED( hr ) )
		{
			throw AudioException( NoAudioClient, hr );
		}

		hr = pAudioClient->Initialize(
			AUDCLNT_SHAREMODE_EXCLUSIVE,
			AUDCLNT_STREAMFLAGS_EVENTCALLBACK,
			hnsRequestedDuration,
			hnsRequestedDuration,
			&m_pwfx,
			NULL
		);
	}
    if( FAILED( hr ) )
	{
		throw AudioException( ClientInitFailed, hr );
	}

	setLatency( double(hnsRequestedDuration) / 10000.0 );

	// Create an event handle and register it for
	// buffer-event notifications.
	if (hEvent.get() == NULL)
	{
		throw AudioException( EventFailed, E_FAIL );
	}
  
	hr = pAudioClient->SetEventHandle(hEvent.get());
    if( FAILED( hr ) )
	{
		throw AudioException( EventFailed, hr );
	}


    // Get the actual size of the allocated buffer.
    hr = pAudioClient->GetBufferSize(&bufferFrameCount);
    if( FAILED( hr ) )
	{
		throw AudioException( NoBufferSize, hr );
	}

    hr = pAudioClient->GetService(
		IID_IAudioRenderClient,
		(void**)&pRenderClient
	);
    if( FAILED( hr ) )
	{
		throw AudioException( NoRenderClient, hr );
	}

    // Grab the entire buffer for the initial fill operation.
    hr = pRenderClient->GetBuffer(bufferFrameCount, &pData);
    if( FAILED( hr ) )
	{
		throw AudioException( NoBuffer, hr );
	}

    // Load the initial data into the shared buffer.
    hr = LoadData(bufferFrameCount, pData, &flags);
    if( FAILED( hr ) )
	{
		throw AudioException( NoData, hr );
	}

    hr = pRenderClient->ReleaseBuffer(bufferFrameCount, flags);
    if( FAILED( hr ) )
	{
		throw AudioException( NoBuffer, hr );
	}

	// Ask MMCSS to temporarily boost the thread priority
	// to reduce glitches while the low-latency stream plays.
	AVRTtask	hTask;
	if( hTask.get() == NULL )
	{
		throw AudioException( NoTask, E_FAIL );
	}

    hr = pAudioClient->Start();  // Start playing.
    if( FAILED( hr ) )
	{
		throw AudioException( StartFailed, hr );
	}

    // Each loop fills about half of the shared buffer.
    while (flags != AUDCLNT_BUFFERFLAGS_SILENT)
    {
		// Wait for next buffer event to be signaled.
		if( !hEvent.WaitFor(2000) )
		{
			// Event handle timed out after a 2-second wait.
			pAudioClient->Stop();
			throw AudioException( AudioTimeout, ERROR_TIMEOUT );
		}

		// Grab the next empty buffer from the audio device.
		hr = pRenderClient->GetBuffer(bufferFrameCount, &pData);
	    if( FAILED( hr ) )
		{
			pAudioClient->Stop();
			throw AudioException( NoBuffer, hr );
		}
  
		// Load the buffer with data from the audio source.
		hr = LoadData(bufferFrameCount, pData, &flags);
	    if( FAILED( hr ) )
		{
			pAudioClient->Stop();
			throw AudioException( NoData, hr );
		}
  
		hr = pRenderClient->ReleaseBuffer(bufferFrameCount, flags);
	    if( FAILED( hr ) )
		{
			pAudioClient->Stop();
			throw AudioException( NoBuffer, hr );
		}
    }

	// Wait for last data in buffer to play before stopping.
	Sleep((DWORD)(hnsRequestedDuration/REFTIMES_PER_MILLISEC));

	pAudioClient->Stop();  // Stop playing.
}

// --------------------------------------------------------------------- //
// ----- class protected ----------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- class virtuals ------------------------------------------------ //
// --------------------------------------------------------------------- //
   
void AudioThread::ExecuteThread( void )
{
	try
	{
		PlayAudioStream();
	}
	catch( ... )
	{
		m_latency = -1;
	}
}

void LevelMeterThread::ExecuteThread( void )
{
	static size_t	counter = 0;
	const clock_t	rate10ms = CLOCKS_PER_SEC * 10 / 1000;
	const clock_t	rate3s = CLOCKS_PER_SEC * 3;

	clock_t			startCollect, currentClock = clock();
	clock_t			peakLeftTime = 0;
	clock_t			peakRightTime = 0;
	StereoSample<>	displaySample, peakSample;
	size_t			numSamples = 0;

	DrawDevice		hDC( m_targetWindow );

	Brush::SystemBrush	backGroundLeftBrush = Brush::sbWhite;
	Brush::SystemBrush	backGroundRightBrush = Brush::sbWhite;

	Size			clientSize = m_targetWindow->getClientSize();

	double	clipValue = sqrt( double( 0x7FFFFF ) );
	int		clipWidth = clientSize.width *2 / 3;

	hDC.getPen().selectPen( Pen::spBlack );

	peakSample.left = 0;
	peakSample.right = 0;

	while( !terminated )
	{
		startCollect = currentClock;
		displaySample.left = 0;
		displaySample.right = 0;
		if( startCollect - peakLeftTime > rate3s )
		{
			peakSample.left = 0;
			backGroundLeftBrush = Brush::sbWhite;
		}
		if( startCollect - peakRightTime > rate3s )
		{
			peakSample.right = 0;
			backGroundRightBrush = Brush::sbWhite;
		}
		do
		{
			StereoSample<>	currentSample;
			if( m_dataSource.pop( &currentSample ) )
			{
				numSamples++;

				if( displaySample.left < abs(currentSample.left) )
					displaySample.left = abs(currentSample.left);
				if( displaySample.right < abs(currentSample.right) )
					displaySample.right = abs(currentSample.right);
			}
			else
				Sleep( 1 );
			currentClock = clock();
		} while( currentClock - startCollect < rate10ms );

		if( peakSample.left < displaySample.left )
		{
			peakSample.left = displaySample.left;
			peakLeftTime = currentClock;
			if( peakSample.left > 0x7FFFFF )
				backGroundLeftBrush = Brush::sbGray;
		}
		if( peakSample.right < displaySample.right )
		{
			peakSample.right = displaySample.right;
			peakRightTime = currentClock;
			if( peakSample.right > 0x7FFFFF )
				backGroundRightBrush = Brush::sbGray;
		}

// 5-1:(0.3*x+0.2)

		if( displaySample.left )
		{
			double	left = sqrt( double( displaySample.left ) );
			int		leftWidth = int(left * double(clipWidth) / clipValue);
			hDC.getBrush().selectBrush( Brush::sbBlack );
			hDC.rectangle( 0, 0, leftWidth, clientSize.height/2 );

			hDC.getBrush().selectBrush( backGroundLeftBrush );
			hDC.rectangle( leftWidth, 0, clientSize.width, clientSize.height/2 );
		}
		else
		{
			hDC.getBrush().selectBrush( backGroundLeftBrush );
			hDC.rectangle( 0, 0, clientSize.width, clientSize.height/2 );
		}
		if( displaySample.right )
		{
			double	right = sqrt( double( displaySample.right ) );
			int		rightWidth = int(right * double(clipWidth) / clipValue);
			hDC.getBrush().selectBrush( Brush::sbBlack );
			hDC.rectangle( 0, clientSize.height/2, rightWidth, clientSize.height );

			hDC.getBrush().selectBrush( backGroundRightBrush );
			hDC.rectangle( rightWidth, clientSize.height/2, clientSize.width, clientSize.height );
		}
		else
		{
			hDC.getBrush().selectBrush( backGroundRightBrush );
			hDC.rectangle( 0, clientSize.height/2, clientSize.width, clientSize.height );
		}

		if( peakSample.left )
		{
			double	left = sqrt( double( peakSample.left ) );
			int		peakPos = int(left * double(clipWidth) / clipValue);

			hDC.line( peakPos, 0, peakPos, clientSize.height/2 );
		}
		if( peakSample.right )
		{
			double	right = sqrt( double( peakSample.right ) );
			int		peakPos = int(right * double(clipWidth) / clipValue);

			hDC.line( peakPos, clientSize.height/2, peakPos, clientSize.height );
		}

		hDC.line( clipWidth, 0, clipWidth, clientSize.height );
	}
}

// --------------------------------------------------------------------- //
// ----- class publics ------------------------------------------------- //
// --------------------------------------------------------------------- //

// --------------------------------------------------------------------- //
// ----- entry points -------------------------------------------------- //
// --------------------------------------------------------------------- //

bool isAudioRunning( void )
{
	return theAudioThread && theAudioThread->isRunning;
}

clock_t	testCPUspeed( void )
{
	size_t	oldOpenCount = openCount;

	if( openCount )
	{
		openCount = 1;
		closeSynthesizer();
	}
	AudioThread::s_synthesizer.setup( 0x7FFFFF, bitsPerSample, sampleRate );
	std::clock_t cpuTime = AudioThread::s_synthesizer.testCPUspeed();

	if( oldOpenCount )
	{
		openSynthesizer();
		openCount = oldOpenCount;
	}

	return cpuTime;
}

bool openSynthesizer( void )
{
	bool	success;
	AudioThread::s_synthesizer.setup( 0x7FFFFF, bitsPerSample, sampleRate );

	if( !theAudioThread || !theAudioThread->isRunning )
	{
		theAudioThread = new AudioThread( bitsPerSample, sampleRate );
		theAudioThread->StartThread();
		Sleep(100);
		if( theAudioThread->isRunning )
		{
			success = true;
		}
		else
		{
			success = false;
			theAudioThread = NULL;
		}
	}
	else
	{
		success = true;
	}

	if( success )
	{
		if( !meterWindow )
		{
			meterWindow = new ToolWindow( NULL );
			meterWindow->addExStyle( WS_EX_TOPMOST );
			meterWindow->create( NULL, 256, 100, "Main Level" );
			meterWindow->restoreWindowPos();
		}
		else
		{
			meterWindow->show();
		}

		if( !statusWindow )
		{
			statusWindow = new StatusWindow<>(AudioThread::s_synthesizer);
			statusWindow->addExStyle( WS_EX_TOPMOST );
			statusWindow->create( NULL, StatusWindow<>::COL_WIDTH*16+40, 500, "Status" );
			statusWindow->restoreWindowPos();
		}
		else
		{
			statusWindow->show();
		}

		if( !theMeterThread || !theMeterThread->isRunning )
		{
			AudioThread::s_meterBuffer.clear();
			theMeterThread = new LevelMeterThread( AudioThread::s_meterBuffer, meterWindow );
			theMeterThread->StartThread();
		}

		if( !theStatusThread || !theStatusThread->isRunning )
		{
			theStatusThread = new StatusThread<>( statusWindow );
			theStatusThread->StartThread();
		}

		openCount++;
	}

	return success;
}

double getLatency( void )
{
	if( theAudioThread )
	{
		return theAudioThread->getLatency();
	}
	else
	{
		return -1;
	}
}

MidiSynthesizer<> &getSyntesizer( void )
{
	return AudioThread::s_synthesizer;
}


/*
	play a midi message
*/

void synthesizerMsg( unsigned long msgCode )
{
	MIDIevent myEvent( 0, msgCode );

	unsigned char msg = myEvent.getMessage();
	if( msg == MIDI_NOTE_OFF )
	{
		unsigned char note = myEvent.getData1();
		AudioThread::s_synthesizer.release( note );
	}
	else if( msg == MIDI_NOTE_ON )
	{
		unsigned char note = myEvent.getData1();
		unsigned char velocity = myEvent.getData2();

		if( !velocity )
		{
			AudioThread::s_synthesizer.release( note );
		}
		else
		{
			AudioThread::s_synthesizer.start( note, velocity );
		}
	}
	else if( msg == MIDI_CONTROLLER )
	{
		unsigned char controller = myEvent.getData1();
		if( controller == MIDI_SOUND_OFF || controller == MIDI_NOTES_OFF )
		{
			AudioThread::s_synthesizer.stop();
		}
	}
#if 0
	else if( msg == MIDI_PITCH )
	{
		short pitchValue = short(myEvent.getShortData()) - MIDI_NEUTRAL_PITCH;
	}
#endif
}

void closeSynthesizer()
{
	if( openCount )
	{
		openCount--;
	}

	if( !openCount )
	{
		if( theMeterThread )
		{
			theMeterThread->StopThread();
			theMeterThread->join();
			theMeterThread = NULL;
		}
		if( theStatusThread )
		{
			theStatusThread->StopThread();
			theStatusThread->join();
			theStatusThread = NULL;
		}
		if( theAudioThread )
		{
			AudioThread::s_synthesizer.stop();
			theAudioThread->StopThread();
			theAudioThread->join();
			theAudioThread = NULL;
		}
		Sleep( 10 );
		Thread::CheckThreadCount();
		if( meterWindow )
		{
			meterWindow->saveWindowRect();
			meterWindow->hide();
		}
		if( statusWindow )
		{
			statusWindow->saveWindowRect();
			statusWindow->hide();
		}
	}
}

#ifdef __BORLANDC__
#	pragma option -RT.
#	pragma option -b.
#	pragma option -a.
#	pragma option -p.
#endif

