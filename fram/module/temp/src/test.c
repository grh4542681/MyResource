#include <stdio.h>
#include <stdlib.h>

int main()
{

	char buf[]="DHE-RSA-AES128-GCM-SHA256 TLSv1.2 Kx=DH       Au=RSA  Enc=AESGCM(128) Mac=AEAD";
	char match[]="%s%*[^A-Za-z0-9]%s%*[^A-Za-z0-9]Kx=%s%*[^A-Za-z0-9]Au=%s%*[^A-Za-z0-9]Enc=%s%*[^A-Za-z0-9]Mac=%s";
	char name[128], ver[128], mkey[128], auth[128], enc[128], mac[128];
   memset(name, 0, sizeof(name));
   memset(ver, 0, sizeof(ver));
   memset(mkey, 0, sizeof(mkey));
   memset(auth, 0, sizeof(auth));
   memset(enc, 0, sizeof(enc));
   memset(mac, 0, sizeof(mac));
	sscanf(buf, match, name, ver, mkey, auth, enc, mac);

	printf("name=%s ver=%s mk=%s auth=%s enc=%s mac=%s\n",name, ver, mkey, auth, enc, mac);

	return 0;
	
}
