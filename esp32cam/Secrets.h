#include <pgmspace.h>
 
#define SECRET
#define THINGNAME "camera"                         //change this
 
const char WIFI_SSID[] = "F1603-4G";               //change this
const char WIFI_PASSWORD[] = "Bel@12345";           //change this
const char AWS_IOT_ENDPOINT[] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxx.amazonaws.com";       //change this
 
// Amazon Root CA 1
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUNMsC3WIaJVdJqc7jt+Yf4tFQchgwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI0MDUwMTIxMTA1
OVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALMIyhkB0saCKkDyTNEX
6yUTOZ4mKoEqoQXHFqFz1PvOkJCnLuM6ah9FMDVTsQ+9LKKCpiNdLL92HjKVCd99
H9u5zWdjwj/7P9Z2rITAZE6bDUtjziwddzxPDhPL/FZCK8CexWS3TFckA0O42Omd
KhdRu0bs7SEzK5u7J5lqWyfnwfsGMvK0b5nHdkrliVuhkVjGwROTT5nhoA9TGyRI
cL3OHeTIlwuz5UtwodfSLtZW9z4RyPdqxF5n1GnkK4BPFWZElNXbx+4iLSOMH/jt
FnoXmvuBb1kqkV3znvsBCe5yxr8dGbbirDxCvUafg2cbkDFitwNCvP14axSgQAYm
ToUCAwEAAaNgMF4wHwYDVR0jBBgwFoAUlU4rPMwXYSi1DgTPZFQpXVc1PDgwHQYD
VR0OBBYEFDCL5zvLc5Qcy4O7UxNrC3MjjxmtMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAyG0Q7tYM3fHM20wmzybzhx7f6
rnCjltgDH/kPK2q054sBAgkVA5fErgFLApy9hIcGwPzrzn6z/ufmX2lwjD4+HERd
Y8Ih0aSDf32nNWxVYe2LRct/g0/K4Z2jVGpMvBXbv60qWhy13N/m1lL4ZcKSv31+
JwyXmROrgIax5ycDFIkQNLl1sdMqc9iIp/dQCt79/fX5kRzCSuMqon1g4cf+lK7S
TH9SZmn0rcrABDgVctuqU6KS9N4rVHMHvL0PYwWSR+IgPnY+MhA9GxV8fgnGL3bn
RJ2j0gtuAq4iTDcUoya4zH3jV9oiJv/5p4MlOOeuR9zXu+EVfkZ174RZNJ3c
-----END CERTIFICATE-----


 
 
)KEY";
 
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAswjKGQHSxoIqQPJM0RfrJRM5niYqgSqhBccWoXPU+86QkKcu
4zpqH0UwNVOxD70sooKmI10sv3YeMpUJ330f27nNZ2PCP/s/1nashMBkTpsNS2PO
LB13PE8OE8v8VkIrwJ7FZLdMVyQDQ7jY6Z0qF1G7RuztITMrm7snmWpbJ+fB+wYy
8rRvmcd2SuWJW6GRWMbBE5NPmeGgD1MbJEhwvc4d5MiXC7PlS3Ch19Iu1lb3PhHI
92rEXmfUaeQrgE8VZkSU1dvH7iItI4wf+O0Wehea+4FvWSqRXfOe+wEJ7nLGvx0Z
tuKsPEK9Rp+DZxuQMWK3A0K8/XhrFKBABiZOhQIDAQABAoIBAFgYYxmbBBQZyKDc
mV/p3RMJOrjPHusgBsvJAyG+1XZyVkBXg4LcI4vvh6E15VJRS71LfGS6XGJ5xLIv
FASclSiRbLkLJXJGN0tYaduIr9LeCNSn77NVbG6Il+y12xy2wsCtX4QHjdDzL4gL
VPDfHO8MkJJ90tEbVMH0fa/H3cLsPAoT+NzBexBJPjpK9J5cX35FAbFJfI2TXoO1
EeOfretPgYgo9/j3cjF9DuHxNlG2ItOe68fn8dJFoO3npphlImHWnRto8TKDsKQR
xO/KmbfVOl19EyoEv09SOegvBN0r83LrrHNgYhhVsokdxDJs+9ju/29nH6bkQzAY
pckXWwECgYEA5piCe4NRGBZ52XvVdx8NjX0jJg1hpaaK1KtCbSlRBjSZwYfayfz4
rbHTwiERGXUZ04Zdqlgrw1FZ9TgfBnXp34GAl/Nk4EadKvCUG4LXRndGN/p6aKxc
XA5cq6Ck5+6jGvnkpP0yL7OQ7+q4ANAa5ZnZqtM2cgAf+KVjVJ3hwvkCgYEAxsIX
++QIhvBwXPSWJRaHdSKKpbUBeoSEvd784l5oQd4CBi+AP7y/UfLIoLjZZ6mqdsWZ
Ee4HnJBkiNv3aDtESpr2Ktkk8Xh8J+y1xVJ2A9BmrRiX4dkB9OG1fPSMrRyS45pL
1LF1YXlTya9Z6LV6G2X6BmvSABDwNtRgHqTwvu0CgYEAhCgz4AXKnDuFaheGp9as
NcLaOhzBMebY+jmMwHNFdIEA/eGWitzoyON5sXCyFB0AyDHhzJbrve0J2Xhoj5NU
jjvyLyPXVJIj0VBAEeRS7Je/LqXnMcZWyIuSoOpYmHQ0FmnRmJ1j/eKz1kx3WD/s
qloaXcTfF0brC939p6k0kkECgYBzfA0xd5KYzRWMfTTMQeXc4oAKvjwostzp8amf
4djUBKnKbEkpP4Mum1GFSUJQ4B1Cu5cv+s0DZEZ12tFLB7yrWJSV2uuDPk1taTVI
fJxDpRAznSmKuWtQnOC1xOYEI6kZg+pxn9ajFzre4frsop1bVApNNtKi5TSdtxJO
W+niFQKBgQDeNmKdkTDmmTcoNmzZcX1QmL4YM6fadUj7Mxq9HiEs4q5t2/Kx9gFG
8jmiREAEtJHXIXXZuawt3EvUj4+exJo7qDs90h0QDDiMHdBsocFLeDc7vQ8YBBRp
iPAsVFcHy+nbC26E4l+drMHyRQME35DQpUDGeV6f8I337yoOWoVjmA==
-----END RSA PRIVATE KEY-----



 
 
)KEY";
