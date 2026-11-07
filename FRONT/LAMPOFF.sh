#Dahua
#curl -g --digest -u admin:CenturiaUA "http://192.168.1.11/cgi-bin/configManager.cgi?action=setConfig&Lighting[0][0].Mode=Off"
#UNV
curl -g --digest -u admin:Centuria2025! -X PUT -H 'Content-Type: application/json' -d '{"Enable":0,"Type":2,"Mode":0,"Num":1,"LampList":[{"Type":6,"Num":4,"PropertyList":[{"Index":0,"Property":0,"Level":0},{"Index":1,"Property":1,"Level":0},{"Index":2,"Property":2,"Level":0},{"Index":3,"Property":3,"Level":0}]}]}' "http://192.168.1.11/LAPI/V1.1/Channels/0/Image/LampCtrl/Video"
#Hikvision
#curl -X PUT --digest -u admin:CenturiaUA -d "<SupplementLight><supplementLightMode>irLight</supplementLightMode><irLightBrightness>0</irLightBrightness></SupplementLight>" "http://192.168.1.11/ISAPI/Image/channels/1"