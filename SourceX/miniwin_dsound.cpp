#include "pch.h"

HRESULT __stdcall DirectSound::QueryInterface(REFIID refiid, LPVOID *lpvoid)
{
	return DS_OK;
};

ULONG __stdcall DirectSound::AddRef()
{
	return 0;
};

ULONG __stdcall DirectSound::Release()
{
	Mix_CloseAudio();
	return 0;
};

HRESULT __stdcall DirectSound::CreateSoundBuffer(LPCDSBUFFERDESC pcDSBufferDesc, LPDIRECTSOUNDBUFFER *ppDSBuffer, LPUNKNOWN pUnkOute)
{
	DUMMY();
	return DS_OK;
};

HRESULT __stdcall DirectSound::GetCaps(LPDSCAPS pDSCaps)
{
	return DS_OK;
};

HRESULT __stdcall DirectSound::DuplicateSoundBuffer(LPDIRECTSOUNDBUFFER pDSBufferOriginal, LPDIRECTSOUNDBUFFER *ppDSBufferDuplicate)
{
	DUMMY();
	return DS_OK;
};

HRESULT __stdcall DirectSound::SetCooperativeLevel(HWND hwnd, DWORD dwLevel)
{
	return DS_OK;
};
