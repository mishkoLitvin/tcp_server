#Dahua
#curl -g --digest -u admin:CenturiaUA "http://192.168.1.12/cgi-bin/magicBox.cgi?action=reboot"
#UNV
curl -g --digest -u admin:Centuria2025! -X PUT -H 'Content-Type: application/json' -d '{}' http://192.168.1.12/LAPI/V1.0/System/Reboot
#Hikvision
#curl --digest -u admin:CenturiaUA -X PUT http://192.168.1.12/ISAPI/System/reboot