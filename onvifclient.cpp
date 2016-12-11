
#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <cstring>

#include "soapStub.h"
#include "soapDeviceBindingProxy.h"
#include "soapMediaBindingProxy.h"
#include "soapPTZBindingProxy.h"
#include "wsseapi.h"

using namespace std;

#define DEV_PASSWORD "Supervisor"
#define MAX_HOSTNAME_LEN 128
//#define MAX_LOGMSG_LEN 256 

int _tmain(int argc, _TCHAR* argv[])
{
	bool blSupportPTZ = false;

	char szHostName[MAX_HOSTNAME_LEN] = { 0 };
	char szPTZName[MAX_HOSTNAME_LEN] = { 0 };
	char szMediaName[MAX_HOSTNAME_LEN] = { 0 };

	DeviceBindingProxy proxyDevice;
	MediaBindingProxy proxyMedia;
	PTZBindingProxy proxyPTZ;

	strcat_s(szHostName, "http://192.168.200.90:10080/onvif/device_service");
	strcat_s(szPTZName, "http://192.168.200.90:10080/onvif/PTZ");
	strcat_s(szMediaName, "http://192.168.200.90:10080/onvif/Media");

	proxyDevice.soap_endpoint = szHostName;
	proxyPTZ.soap_endpoint = szPTZName;
	proxyMedia.soap_endpoint = szMediaName;


	soap_register_plugin(&proxyDevice, soap_wsse);
	soap_register_plugin(&proxyMedia, soap_wsse);
	soap_register_plugin(&proxyPTZ, soap_wsse);

	soap_wsse_add_Security(&proxyPTZ);

	
	struct soap *soap1 = soap_new();

	if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(&proxyDevice, NULL, "admin", DEV_PASSWORD))
	{
		return -1;
	}

	if (SOAP_OK != soap_wsse_add_Timestamp(&proxyDevice, "Time", 10))
	{
		return -1;
	}

	_tds__GetWsdlUrl *tds__GetWsdlUrl = soap_new__tds__GetWsdlUrl(&proxyDevice, -1);
	_tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse = soap_new__tds__GetWsdlUrlResponse(&proxyDevice, -1);

	if (SOAP_OK == proxyDevice.GetWsdlUrl(tds__GetWsdlUrl, tds__GetWsdlUrlResponse))
	{
		cout << "-------------------WsdlUrl-------------------"<< endl;
		cout << "WsdlUrl:%s " << tds__GetWsdlUrlResponse->WsdlUrl.c_str() << endl;
	}

	soap_destroy(soap1);
	soap_end(soap1);

	if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(&proxyDevice, NULL, "admin", DEV_PASSWORD))
	{
		return -1;
	}

	_tds__GetCapabilities *tds__GetCapabilities = soap_new__tds__GetCapabilities(soap1, -1);
	tds__GetCapabilities->Category.push_back(tt__CapabilityCategory__All);

	_tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse = soap_new__tds__GetCapabilitiesResponse(soap1, -1);

	if (SOAP_OK == proxyDevice.GetCapabilities(tds__GetCapabilities, tds__GetCapabilitiesResponse))
	{
		if (tds__GetCapabilitiesResponse->Capabilities->Analytics != NULL)
		{
			cout << "-------------------Analytics-------------------" << endl;
			cout << "XAddr: " << tds__GetCapabilitiesResponse->Capabilities->Analytics->XAddr.c_str() << endl;
			cout << "RuleSupport: " << tds__GetCapabilitiesResponse->Capabilities->Analytics->RuleSupport ? "Y" : "N";
			cout << endl;
			cout << "AnalyticsModuleSupport: " << tds__GetCapabilitiesResponse->Capabilities->Analytics->AnalyticsModuleSupport ? "Y" : "N";
			cout << endl;
		}

		if (tds__GetCapabilitiesResponse->Capabilities->Device != NULL)
		{
			cout << "-------------------Device-------------------" << endl;
			cout << "XAddr:", tds__GetCapabilitiesResponse->Capabilities->Device->XAddr.c_str();
			cout << endl;

			cout << "-------------------Network-------------------" << endl;
			cout << "IPFilter: ", (tds__GetCapabilitiesResponse->Capabilities->Device->Network->IPFilter) ? "Y" : "N"; 
			cout << endl;
			cout << "ZeroConfiguration: ", (tds__GetCapabilitiesResponse->Capabilities->Device->Network->ZeroConfiguration) ? "Y" : "N";
			cout << endl;
			cout << "IPVersion6: ", (tds__GetCapabilitiesResponse->Capabilities->Device->Network->IPVersion6) ? "Y" : "N";
			cout << endl;
			cout << "DynDNS: ", (tds__GetCapabilitiesResponse->Capabilities->Device->Network->DynDNS) ? "Y" : "N";
			cout << endl;

			cout << "-------------------System-------------------" << endl;
			cout << "DiscoveryResolve: ", (tds__GetCapabilitiesResponse->Capabilities->Device->System->DiscoveryResolve) ? "Y" : "N";
			cout << endl;
			cout << "DiscoveryBye: ", (tds__GetCapabilitiesResponse->Capabilities->Device->System->DiscoveryBye) ? "Y" : "N";
			cout << endl;
			cout << "RemoteDiscovery: ", (tds__GetCapabilitiesResponse->Capabilities->Device->System->RemoteDiscovery) ? "Y" : "N";
			cout << endl;

			int iSize = tds__GetCapabilitiesResponse->Capabilities->Device->System->SupportedVersions.size();

			if (iSize > 0)
			{
				cout << "SupportedVersions: " << endl;

				for (int i = 0; i < iSize; i++)
				{
					cout << " " << tds__GetCapabilitiesResponse->Capabilities->Device->System->SupportedVersions[i]->Major <<
						tds__GetCapabilitiesResponse->Capabilities->Device->System->SupportedVersions[i]->Minor;
				}

				cout << endl;
			}

			cout << "SystemBackup: " << (tds__GetCapabilitiesResponse->Capabilities->Device->System->SystemBackup) ? "Y" : "N";
			cout << endl;
			cout << "FirmwareUpgrade: " << (tds__GetCapabilitiesResponse->Capabilities->Device->System->FirmwareUpgrade) ? "Y" : "N";
			cout << endl;
			cout << "SystemLogging: " << (tds__GetCapabilitiesResponse->Capabilities->Device->System->SystemLogging) ? "Y" : "N";
			cout << endl;

			cout << "-------------------IO-------------------" << endl;
			cout << "InputConnectors: " << tds__GetCapabilitiesResponse->Capabilities->Device->IO->InputConnectors << endl;
			cout << "RelayOutputs: " << tds__GetCapabilitiesResponse->Capabilities->Device->IO->RelayOutputs << endl;

			cout << "-------------------Security-------------------" << endl;
			cout << "TLS1.1: " << (tds__GetCapabilitiesResponse->Capabilities->Device->Security->TLS1_x002e1) ? "Y" : "N";
			cout << endl;
			cout << "TLS1.2: " << (tds__GetCapabilitiesResponse->Capabilities->Device->Security->TLS1_x002e2) ? "Y" : "N";
			cout << endl;
			cout << "OnboardKeyGeneration: " << (tds__GetCapabilitiesResponse->Capabilities->Device->Security->OnboardKeyGeneration) ? "Y" : "N";
			cout << endl;
			cout << "AccessPolicyConfig: " << (tds__GetCapabilitiesResponse->Capabilities->Device->Security->AccessPolicyConfig) ? "Y" : "N"; 
			cout << endl;
			cout << "X.509Token: " << (tds__GetCapabilitiesResponse->Capabilities->Device->Security->X_x002e509Token) ? "Y" : "N";
			cout << endl;
			cout << "SAMLToken: " << (tds__GetCapabilitiesResponse->Capabilities->Device->Security->SAMLToken) ? "Y" : "N";
			cout << endl;
			cout << "KerberosToken: " << (tds__GetCapabilitiesResponse->Capabilities->Device->Security->KerberosToken) ? "Y" : "N";
			cout << endl;
			cout << "RELToken: " << (tds__GetCapabilitiesResponse->Capabilities->Device->Security->RELToken) ? "Y" : "N";
			cout << endl;
		}

		if (tds__GetCapabilitiesResponse->Capabilities->Events != NULL)
		{
			cout << "-------------------Events-------------------" << endl;
			cout << "XAddr: " << tds__GetCapabilitiesResponse->Capabilities->Events->XAddr.c_str() << endl;
			cout << "WSSubscriptionPolicySupport: " << (tds__GetCapabilitiesResponse->Capabilities->Events->WSSubscriptionPolicySupport) ? "Y" : "N";
			cout << endl;
			cout << "WSPullPointSupport: " << (tds__GetCapabilitiesResponse->Capabilities->Events->WSPullPointSupport) ? "Y" : "N";
			cout << endl;
			cout << "WSPausableSubscriptionManagerInterfaceSupport: " <<
				(tds__GetCapabilitiesResponse->Capabilities->Events->WSPausableSubscriptionManagerInterfaceSupport) ? "Y" : "N";
			cout << endl;
		}

		if (tds__GetCapabilitiesResponse->Capabilities->Imaging != NULL)
		{
			cout << "-------------------Imaging-------------------" << endl;
			cout << "XAddr: " << tds__GetCapabilitiesResponse->Capabilities->Imaging->XAddr.c_str() << endl;
		}

		if (tds__GetCapabilitiesResponse->Capabilities->Media != NULL)
		{
			cout << "-------------------Media-------------------" << endl;
			cout << "XAddr: " << tds__GetCapabilitiesResponse->Capabilities->Media->XAddr.c_str() << endl;

			cout << "-------------------streaming-------------------" << endl;
			cout << "RTPMulticast: " << (tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->RTPMulticast) ? "Y" : "N";
			cout << endl;
			cout << "RTP_TCP: " << (tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->RTP_USCORETCP) ? "Y" : "N";
			cout << endl;
			cout << "RTP_RTSP_TCP: " << (tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP) ? "Y" : "N";
			cout << endl;

			//proxyMedia.soap_endpoint = tds__GetCapabilitiesResponse->Capabilities->Media->XAddr.c_str();
		}

		if (tds__GetCapabilitiesResponse->Capabilities->PTZ != NULL)
		{
			cout << "-------------------PTZ-------------------" << endl;
			cout << "XAddr: " << tds__GetCapabilitiesResponse->Capabilities->PTZ->XAddr.c_str() << endl;

			//proxyPTZ.soap_endpoint = tds__GetCapabilitiesResponse->Capabilities->PTZ->XAddr.c_str();
			blSupportPTZ = true;
			cout << endl;
		}
	}

	soap_destroy(soap1);
	soap_end(soap1);


	if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(&proxyDevice, NULL, "admin", DEV_PASSWORD))
	{
		return -1;
	}

	_tds__GetDeviceInformation *tds__GetDeviceInformation = soap_new__tds__GetDeviceInformation(soap1, -1);
	_tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse = soap_new__tds__GetDeviceInformationResponse(soap1, -1);

	if (SOAP_OK == proxyDevice.GetDeviceInformation(tds__GetDeviceInformation, tds__GetDeviceInformationResponse))
	{
		cout << "-------------------DeviceInformation-------------------" << endl;
		cout << "Manufacturer: " << tds__GetDeviceInformationResponse->Manufacturer.c_str();
		cout << endl;   
		cout << "Model: " << tds__GetDeviceInformationResponse->Model.c_str();
		cout << endl;
		cout << "FirmwareVersion: " << tds__GetDeviceInformationResponse->FirmwareVersion.c_str();
		cout << endl;
		cout << "SerialNumber: " << tds__GetDeviceInformationResponse->SerialNumber.c_str();
		cout << endl;
		cout << "HardwareId: " << tds__GetDeviceInformationResponse->HardwareId.c_str();
		cout << endl;
	}

	soap_destroy(soap1);
	soap_end(soap1);
	
	if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(&proxyDevice, NULL, "admin", DEV_PASSWORD))
	{
		return -1;
	}

	_tds__GetNetworkInterfaces *tds__GetNetworkInterfaces = soap_new__tds__GetNetworkInterfaces(soap1, -1);
	_tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse = soap_new__tds__GetNetworkInterfacesResponse(soap1, -1);

	if (SOAP_OK == proxyDevice.GetNetworkInterfaces(tds__GetNetworkInterfaces, tds__GetNetworkInterfacesResponse))
	{
		cout << "-------------------NetworkInterfaces-------------------" << endl;
		cout << tds__GetNetworkInterfacesResponse->NetworkInterfaces[0]->token.c_str();
		cout << endl;
		cout << tds__GetNetworkInterfacesResponse->NetworkInterfaces[0]->Info->HwAddress.c_str();
		cout << endl;
	}

	soap_destroy(soap1);
	soap_end(soap1);

	if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(&proxyMedia, NULL, "admin", DEV_PASSWORD))
	{
		return -1;
	}

	if (SOAP_OK != soap_wsse_add_Timestamp(&proxyMedia, "Time", 10))
	{
		return -1;
	}

	_trt__GetProfiles *trt__GetProfiles = soap_new__trt__GetProfiles(soap1, -1);
	_trt__GetProfilesResponse *trt__GetProfilesResponse = soap_new__trt__GetProfilesResponse(soap1, -1);

	if (SOAP_OK == proxyMedia.GetProfiles(trt__GetProfiles, trt__GetProfilesResponse))
	{
		_trt__GetStreamUri *trt__GetStreamUri = soap_new__trt__GetStreamUri(soap1, -1);
		trt__GetStreamUri->StreamSetup = soap_new_tt__StreamSetup(soap1, -1);
		trt__GetStreamUri->StreamSetup->Stream = tt__StreamType__RTP_Unicast;
		trt__GetStreamUri->StreamSetup->Transport = soap_new_tt__Transport(soap1, -1);
		trt__GetStreamUri->StreamSetup->Transport->Protocol = tt__TransportProtocol__RTSP;

		_trt__GetStreamUriResponse *trt__GetStreamUriResponse = soap_new__trt__GetStreamUriResponse(soap1, -1);

		cout << "-------------------MediaProfiles-------------------" << endl;
		for (int i = 0; i < trt__GetProfilesResponse->Profiles.size(); i++)
		{
			cout << "profile, Token " << i << ":    " << trt__GetProfilesResponse->Profiles[i]->Name.c_str();
			cout << " ";
			cout << trt__GetProfilesResponse->Profiles[i]->token.c_str();
			cout << endl;

			if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(&proxyMedia, NULL, "admin", DEV_PASSWORD))
			{
				return -1;
			}

			if (SOAP_OK == proxyMedia.GetStreamUri(trt__GetStreamUri, trt__GetStreamUriResponse))
			{
				cout << "RTSP URI:", trt__GetStreamUriResponse->MediaUri->Uri.c_str();
				cout << endl;
			}
		}
	}

	soap_destroy(soap1);
	soap_end(soap1);


	if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(&proxyPTZ, NULL, "admin", "Supervisor"))
	{
		return -1;
	}

	if (SOAP_OK != soap_wsse_add_Timestamp(&proxyPTZ, "Time", 10))
	{
		return -1;
	}

	_tptz__GetConfigurations *tptz__GetConfigurations = soap_new__tptz__GetConfigurations(soap1, -1);
	_tptz__GetConfigurationsResponse *tptz__GetConfigurationsResponse = soap_new__tptz__GetConfigurationsResponse(soap1, -1);


	if (SOAP_OK == proxyPTZ.GetConfigurations(tptz__GetConfigurations, tptz__GetConfigurationsResponse))
	{
		cout << "------------------PTZ Configurations-------------" << endl;
		for (int i = 0; i < tptz__GetConfigurationsResponse->PTZConfiguration.size(); i++)
		{
			cout << "Config" << i + 1 << ": " << endl;
			cout << tptz__GetConfigurationsResponse->PTZConfiguration[i]->NodeToken;
			cout << endl;
			cout << "-----------------Conf options-------------------" << endl;
			cout << "PanTilt Max x: " << tptz__GetConfigurationsResponse->PTZConfiguration[i]->PanTiltLimits->Range->XRange->Max << endl;
			cout << "PanTilt Max y: " << tptz__GetConfigurationsResponse->PTZConfiguration[i]->PanTiltLimits->Range->YRange->Max << endl;
			cout << "PanTilt URI: " << tptz__GetConfigurationsResponse->PTZConfiguration[i]->PanTiltLimits->Range->URI << endl;
			cout << "Zoom Max x: " << tptz__GetConfigurationsResponse->PTZConfiguration[i]->ZoomLimits->Range->XRange->Max << endl;
			cout << "Zoom URI: " << tptz__GetConfigurationsResponse->PTZConfiguration[i]->ZoomLimits->Range->URI << endl;
		}

		/*_tptz__ContinuousMove *tptz__ContinuousMove = soap_new__tptz__ContinuousMove(soap1, -1);
		_tptz__ContinuousMoveResponse *tptz__ContinuousMoveResponse = soap_new__tptz__ContinuousMoveResponse(soap1, -1);

		tt__PTZSpeed *Speed = soap_new_tt__PTZSpeed(soap1, -1);
		Speed->PanTilt = new tt__Vector2D;
		Speed->Zoom = new tt__Vector1D;

		Speed->PanTilt->x = -0.5;
		Speed->PanTilt->y = 0.0;
		Speed->Zoom->x = 0.0;
		
		tptz__ContinuousMove->Velocity = Speed;
		tptz__ContinuousMove->ProfileToken = "Profile_1";

		LONG64 timeout = 2000;
		tptz__ContinuousMove->Timeout = &timeout;

		_tptz__GotoHomePosition *tptz__GotoHomePosition = soap_new__tptz__GotoHomePosition(soap1, -1);
		_tptz__GotoHomePositionResponse *tptz__GotoHomePositionResponse = soap_new__tptz__GotoHomePositionResponse(soap1, -1);

		tptz__GotoHomePosition->ProfileToken = "Profile_1";

		if (SOAP_OK != soap_wsse_add_UsernameTokenDigest(&proxyPTZ, NULL, "admin", "Supervisor"))
		{
			return -1;
		}*/

		/*if (SOAP_OK == proxyPTZ.GotoHomePosition(tptz__GotoHomePosition, tptz__GotoHomePositionResponse))
		{
			cout << "GUT";
		}*/
		/*if (SOAP_OK == proxyPTZ.ContinuousMove(tptz__ContinuousMove, tptz__ContinuousMoveResponse))
		{
 			cout << "GUT";
		}*/
	}
	getchar();

	soap_destroy(soap1);
	soap_end(soap1);

	/*_tptz__GetConfiguration *tptz__GetConfiguration = soap_new__tptz__GetConfiguration(soap1, -1);
	_tptz__GetConfigurationResponse *tptz__GetConfigurationResponse = soap_new__tptz__GetConfigurationResponse(soap1, -1);
	

	if (SOAP_OK == proxyPTZ.GetConfiguration(tptz__GetConfiguration, tptz__GetConfigurationResponse))
	{
		cout << "-------------------PTZ_Config-------------------" << endl;
		cout << tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZSpeed->PanTilt->x;
		cout << endl;
		cout << tptz__GetConfigurationResponse->PTZConfiguration->DefaultPTZSpeed->PanTilt->y;
		cout << endl;
	}

	getchar();

	_tptz__AbsoluteMove *tptz__AbsoluteMove = soap_new__tptz__AbsoluteMove(soap1, -1);
	_tptz__AbsoluteMoveResponse *tptz__AbsoluteMoveResponse = soap_new__tptz__AbsoluteMoveResponse(soap1, -1);

	proxyPTZ.AbsoluteMove(tptz__AbsoluteMove, tptz__AbsoluteMoveResponse);
	*/

	

	
	/*
	int ret;
	//192.168.1.1 is the NVT, 192.168.1.234 is the NVC
	string url = "http://92.242.58.67:10080/onvif/device_service";
	//Below is where to receive the event
	string eventNotify = "http://192.168.1.234:9090/subscription-2";
	string user = "admin";
	string pass =  "Supervisor";
	OnvifClientDevice onvifDevice(url, user, pass);
	_tds__GetCapabilitiesResponse capabilitiesResponse;
	onvifDevice.GetCapabilities(capabilitiesResponse);

	struct soap *soap1 = soap_new();

	strcat_s(szHostName, "http://92.242.58.67:10080/onvif/PTZ");
	PTZBindingProxy ptzProxy;
	string token = "PTZToken";
	ptzProxy.soap_endpoint = szHostName;
	tt__PTZVector *vector1 = soap_new_tt__PTZVector(soap1);
	tt__PTZSpeed *speed = soap_new_tt__PTZSpeed(soap1);

	_tptz__AbsoluteMove *move = soap_new_set__tptz__AbsoluteMove(soap1, token, vector1, speed);
	_tptz__AbsoluteMoveResponse *response = soap_new_set__tptz__AbsoluteMoveResponse(soap1);
	if (SOAP_OK == ptzProxy.AbsoluteMove(ptzProxy.soap_endpoint, NULL, move, response))
	{
		cout << "dsdfsdf";
	}*/
	
	/*onvifDevice.GetCapabilities();

	OnvifClientMedia media(onvifDevice);
	_trt__GetProfilesResponse profiles;
	//media.GetProfiles(profiles);

	OnvifClientEvent onvifEvent(onvifDevice);

	onvifEvent.Subscribe(eventNotify);

	OnvifClientEventNotify notify(soap_new());

	if ((ret = soap_bind(&notify, NULL, 10080, 100) == SOAP_INVALID_SOCKET))
	{
		printf("OnvifClientEventNotify::soap_bind Binding on %d port failed", 9090);
		return 0;
	}
	//Loop to recevie the event
	while (1)
	{
		if ((ret = soap_accept(&notify)) == SOAP_INVALID_SOCKET) {
			printf("soap_accept accepting failed");
			return 0;
		}

		if ((soap_begin_serve(&notify)) != SOAP_OK) {
			printf("soap_begin_serve serve %d failed", ret);
			continue;
		}

		ret = notify.dispatch();
		continue;
	}*/
		//OnvifClientMedia media(onvifDevice);
		//_trt__GetProfilesResponse profiles;
		//media.GetProfiles(profiles);
		/*
		// Recording
		OnvifClientRecording onvifRecording(onvifDevice);
		_trc__GetRecordingsResponse recordings;
		onvifRecording.GetRecordings(recordings);
		for (vector<tt__GetRecordingsResponseItem * >::iterator iter =
				recordings.RecordingItem.begin();
				iter < recordings.RecordingItem.end(); ++iter)
		{

			tt__GetRecordingsResponseItem * pRecordings = *iter;
			printf("pRecordings %s\n", pRecordings->RecordingToken.c_str());
			tt__RecordingConfiguration *pRecordingConf = pRecordings->Configuration;
			if (pRecordingConf->Source)
			{
				printf("Recording SourceId %s\n", pRecordingConf->Source->SourceId.c_str());
				printf("Recording Name %s\n", pRecordingConf->Source->Name.c_str());
				printf("Recording Location %s\n", pRecordingConf->Source->Location.c_str());
				printf("Recording Description %s\n", pRecordingConf->Source->Description.c_str());
				printf("Recording Address %s\n", pRecordingConf->Source->Address.c_str());
			}

			string replayUrl;
			OnvifClientReplay onvifRelay(onvifDevice);
			onvifRelay.GetReplayUri(pRecordings->RecordingToken, replayUrl);
			printf("Relay Url %s\n", replayUrl.c_str());
		}

		// Onvif Receiver
		OnvifClientReceiver onvifReceiver(onvifDevice);
		_trv__GetReceiversResponse receivers;
		onvifReceiver.GetReceivers(receivers);
		for (vector<tt__Receiver * >::iterator iter =
				receivers.Receivers.begin();
				iter < receivers.Receivers.end(); ++iter)
		{
			tt__Receiver * pReceivers = *iter;
			printf("pReceivers %s\n", pReceivers->Token.c_str());
		}

		OnvifClientEvent onvifEvent(onvifDevice);

		onvifEvent.Subscribe(eventNotify);

		OnvifClientEventNotify notify(soap_new());

		if ((ret = soap_bind(&notify, NULL, 9090, 100) == SOAP_INVALID_SOCKET ) )
		{
			printf("OnvifClientEventNotify::soap_bind Binding on %d port failed", 9090);
			return 0;
		}
		int timeStart = time(NULL);
		int currentTime = 0;

		//Loop to recevie the event
		while(1)
		{
			currentTime = time(NULL);
			if (currentTime - timeStart > 30)
			{
				onvifEvent.Renew();
			}
			//printf("soap_accept accepting\n");
			if( (ret = soap_accept(&notify)) == SOAP_INVALID_SOCKET) {
				//printf("soap_accept accepting timeout\n");
				continue;
			}

			if ( (soap_begin_serve(&notify)) != SOAP_OK) {
				printf("soap_begin_serve serve %d failed", ret);
				continue;
			}

			ret = notify.dispatch();
			continue;
		}*/
		return 0;
}

