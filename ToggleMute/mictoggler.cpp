#include <Windows.h>
#include <stdio.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

/* Toggles mute state of the default audio capture device using the Windows Multimedia Device API */

HRESULT toggleMuteStateCOM() {

	IMMDeviceEnumerator* deviceEnumerator = nullptr;
	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&deviceEnumerator));
	if (SUCCEEDED(hr)) {
		IMMDevice* defaultDevice = nullptr;
		hr = deviceEnumerator->GetDefaultAudioEndpoint(eCapture, eMultimedia, &defaultDevice);
		if (SUCCEEDED(hr)) {
			IAudioEndpointVolume* endpointVolume = nullptr;
			hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, nullptr, reinterpret_cast<void**>(&endpointVolume));
			if (SUCCEEDED(hr)) {
				BOOL isDefaultDeviceMuted;
				hr = endpointVolume->GetMute(&isDefaultDeviceMuted);
				if (SUCCEEDED(hr))
					hr = endpointVolume->SetMute(!isDefaultDeviceMuted, nullptr);
				endpointVolume->Release();
			}
			defaultDevice->Release();
		}
		deviceEnumerator->Release();
	}
	return hr;
}

extern "C" HRESULT toggleMuteState() {

	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED); // use COINIT_APARTMENTTHREADED in a GUI app
	if (SUCCEEDED(hr)) {
		hr = toggleMuteStateCOM();
		CoUninitialize();
	}

	if (FAILED(hr))
		printf("Failed to toggle mute state of default audio capture device: %x\n", hr);

	return hr;
}
