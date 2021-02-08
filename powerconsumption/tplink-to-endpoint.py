from tplinkcloud import TPLinkDeviceManager
import requests
import time

username='EMAIL'
password='PASSWORD'

endpoint = 'ENDPOINT URL'

device_name = 'DEVICENAME'

device_manager = TPLinkDeviceManager(username, password)

devices = device_manager.get_devices()
if devices:
  print(f'Found {len(devices)} devices')
  for device in devices:
    print(f'{device.model_type.name} device called {device.get_alias()}')
    
# if device:
 # print(f'Found {device.model_type.name} device: {device.get_alias()}')
 # device.toggle()
  
# import json
#print(json.dumps(power_usage, indent=2, default=lambda x: x.__dict__))

while True:
    try:
        power_usage = device_manager.find_device(device_name).get_power_usage()
        load = float(power_usage['power'])
        ret = requests.get(endpoint + '/metrics?device=power&load=' + str(load))
        print(load)
        print(ret)
        time.sleep(5)
    except:
        print('fehler')
