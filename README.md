# zsign
Maybe is the most quickly codesign alternative for iOS12+ in the world, cross-platform ( Linux & macOS ), more features.  
If this tool can help you, please don't forget to star me. :) 

### Compile
You must install openssl library at first,

#### macOS:
```bash
brew install openssl
```
and then (attention to replace your openssl version)
```bash
g++ *.cpp common/*.cpp -lcrypto -I/usr/local/Cellar/openssl/1.0.2s/include -L/usr/local/Cellar/openssl/1.0.2s/lib -O3 -o zsign
```

#### CentOS:
```bash
yum install openssl-devel
```
and then
```bash
g++ *.cpp common/*.cpp -lcrypto -O3 -o zsign
```

### Usage
I have already tested on macOS and Linux, but you also need **unzip** and **zip** command installed.

```bash
Usage: zsign [-options] [-k privkey.pem] [-m dev.prov] [-o output.ipa] file|folder

options:
-k, --pkey          Path to private key or p12 file. (PEM or DER format)
-m, --prov          Path to mobile provisioning profile.
-c, --cert          Path to certificate file. (PEM or DER format)
-d, --debug         Generate debug output files. (.zsign_debug folder)
-f, --force         Force sign without cache when signing folder.
-o, --output        Path to output ipa file.
-p, --password      Password for private key or p12 file.
-b, --bundleid      New bundle id to change.
-n, --bundlename    New bundle name to change.
-e, --entitlements  New entitlements to change.
-z, --ziplevel      Compressed level when output the ipa file. (0-9)
-l, --dylib         Path to inject dylib file.
-w, --weak          Inject dylib as LC_LOAD_WEAK_DYLIB.
-i, --install       Install ipa file using ideviceinstaller command for test.
-q, --quiet         Quiet operation.
-v, --version       Show version.
-h, --help          Show help.
```

1. Show mach-o and codesignature segment info.
```bash
./zsign demo.app/execute
```

2. Sign ipa with private key and mobileprovisioning file.
```bash
./zsign -k privkey.pem -m dev.prov -o output.ipa -z 9 demo.ipa
```

3. Sign folder with p12 and mobileprovisioning file (using cache).
```bash
./zsign -k dev.p12 -p 123 -m dev.prov -o output.ipa demo.app
```

4. Sign folder with p12 and mobileprovisioning file (without cache).
```bash
./zsign -f -k dev.p12 -p 123 -m dev.prov -o output.ipa demo.app
```

5. Inject dylib into ipa and re-sign.
```bash
./zsign -k dev.p12 -p 123 -m dev.prov -o output.ipa -l demo.dylib demo.ipa 
```

6. Change bundle id and bundle name
```bash
./zsign -k dev.p12 -p 123 -m dev.prov -o output.ipa -b 'com.tree.new.bee' -n 'TreeNewBee' demo.ipa
```

7. Inject dylib(LC_LOAD_DYLIB) into mach-o file.
```bash
./zsign -l "@executable_path/demo.dylib" demo.app/execute
```

8. Inject dylib(LC_LOAD_WEAK_DYLIB) into mach-o file.
```bash
./zsign -w -l "@executable_path/demo.dylib" demo.app/execute
```

### Copyright
zsign is completely free. Please mark the source of zsign in your commercial product if possible.

### How to sign quickly?
You can unzip the ipa file at first, and then using zsign to sign folder with assets.  
At the first time of sign, zsign will perform the complete signing and cache the signed info into *.zsign_cache* dir at the current path.  
When you re-sign the folder with other assets next time, zsign will use the cache to accelerate the operation. Extremely fast! You can have a try！：）