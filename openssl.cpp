#include "common/common.h"
#include "common/base64.h"
#include "openssl.h"

#include <openssl/pem.h>
#include <openssl/cms.h>
#include <openssl/err.h>
#include <openssl/pkcs12.h>

class COpenSSLInit
{
public:
	COpenSSLInit()
	{
		OpenSSL_add_all_algorithms();
		ERR_load_crypto_strings();
	};
};

COpenSSLInit g_OpenSSLInit;

const char *appleDevCACert = ""
							 "-----BEGIN CERTIFICATE-----\n"
							 "MIIEIjCCAwqgAwIBAgIIAd68xDltoBAwDQYJKoZIhvcNAQEFBQAwYjELMAkGA1UE\n"
							 "BhMCVVMxEzARBgNVBAoTCkFwcGxlIEluYy4xJjAkBgNVBAsTHUFwcGxlIENlcnRp\n"
							 "ZmljYXRpb24gQXV0aG9yaXR5MRYwFAYDVQQDEw1BcHBsZSBSb290IENBMB4XDTEz\n"
							 "MDIwNzIxNDg0N1oXDTIzMDIwNzIxNDg0N1owgZYxCzAJBgNVBAYTAlVTMRMwEQYD\n"
							 "VQQKDApBcHBsZSBJbmMuMSwwKgYDVQQLDCNBcHBsZSBXb3JsZHdpZGUgRGV2ZWxv\n"
							 "cGVyIFJlbGF0aW9uczFEMEIGA1UEAww7QXBwbGUgV29ybGR3aWRlIERldmVsb3Bl\n"
							 "ciBSZWxhdGlvbnMgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwggEiMA0GCSqGSIb3\n"
							 "DQEBAQUAA4IBDwAwggEKAoIBAQDKOFSmy1aqyCQ5SOmM7uxfuH8mkbw0U3rOfGOA\n"
							 "YXdkXqUHI7Y5/lAtFVZYcC1+xG7BSoU+L/DehBqhV8mvexj/avoVEkkVCBmsqtsq\n"
							 "Mu2WY2hSFT2Miuy/axiV4AOsAX2XBWfODoWVN2rtCbauZ81RZJ/GXNG8V25nNYB2\n"
							 "NqSHgW44j9grFU57Jdhav06DwY3Sk9UacbVgnJ0zTlX5ElgMhrgWDcHld0WNUEi6\n"
							 "Ky3klIXh6MSdxmilsKP8Z35wugJZS3dCkTm59c3hTO/AO0iMpuUhXf1qarunFjVg\n"
							 "0uat80YpyejDi+l5wGphZxWy8P3laLxiX27Pmd3vG2P+kmWrAgMBAAGjgaYwgaMw\n"
							 "HQYDVR0OBBYEFIgnFwmpthhgi+zruvZHWcVSVKO3MA8GA1UdEwEB/wQFMAMBAf8w\n"
							 "HwYDVR0jBBgwFoAUK9BpR5R2Cf70a40uQKb3R01/CF4wLgYDVR0fBCcwJTAjoCGg\n"
							 "H4YdaHR0cDovL2NybC5hcHBsZS5jb20vcm9vdC5jcmwwDgYDVR0PAQH/BAQDAgGG\n"
							 "MBAGCiqGSIb3Y2QGAgEEAgUAMA0GCSqGSIb3DQEBBQUAA4IBAQBPz+9Zviz1smwv\n"
							 "j+4ThzLoBTWobot9yWkMudkXvHcs1Gfi/ZptOllc34MBvbKuKmFysa/Nw0Uwj6OD\n"
							 "Dc4dR7Txk4qjdJukw5hyhzs+r0ULklS5MruQGFNrCk4QttkdUGwhgAqJTleMa1s8\n"
							 "Pab93vcNIx0LSiaHP7qRkkykGRIZbVf1eliHe2iK5IaMSuviSRSqpd1VAKmuu0sw\n"
							 "ruGgsbwpgOYJd+W+NKIByn/c4grmO7i77LpilfMFY0GCzQ87HUyVpNur+cmV6U/k\n"
							 "TecmmYHpvPm0KdIBembhLoz2IYrF+Hjhga6/05Cdqa3zr/04GpZnMBxRpVzscYqC\n"
							 "tGwPDBUf\n"
							 "-----END CERTIFICATE-----\n";

const char *appleRootCACert = ""
							  "-----BEGIN CERTIFICATE-----\n"
							  "MIIEuzCCA6OgAwIBAgIBAjANBgkqhkiG9w0BAQUFADBiMQswCQYDVQQGEwJVUzET\n"
							  "MBEGA1UEChMKQXBwbGUgSW5jLjEmMCQGA1UECxMdQXBwbGUgQ2VydGlmaWNhdGlv\n"
							  "biBBdXRob3JpdHkxFjAUBgNVBAMTDUFwcGxlIFJvb3QgQ0EwHhcNMDYwNDI1MjE0\n"
							  "MDM2WhcNMzUwMjA5MjE0MDM2WjBiMQswCQYDVQQGEwJVUzETMBEGA1UEChMKQXBw\n"
							  "bGUgSW5jLjEmMCQGA1UECxMdQXBwbGUgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkx\n"
							  "FjAUBgNVBAMTDUFwcGxlIFJvb3QgQ0EwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAw\n"
							  "ggEKAoIBAQDkkakJH5HbHkdQ6wXtXnmELes2oldMVeyLGYne+Uts9QerIjAC6Bg+\n"
							  "+FAJ039BqJj50cpmnCRrEdCju+QbKsMflZ56DKRHi1vUFjczy8QPTc4UadHJGXL1\n"
							  "XQ7Vf1+b8iUDulWPTV0N8WQ1IxVLFVkds5T39pyez1C6wVhQZ48ItCD3y6wsIG9w\n"
							  "tj8BMIy3Q88PnT3zK0koGsj+zrW5DtleHNbLPbU6rfQPDgCSC7EhFi501TwN22IW\n"
							  "q6NxkkdTVcGvL0Gz+PvjcM3mo0xFfh9Ma1CWQYnEdGILEINBhzOKgbEwWOxaBDKM\n"
							  "aLOPHd5lc/9nXmW8Sdh2nzMUZaF3lMktAgMBAAGjggF6MIIBdjAOBgNVHQ8BAf8E\n"
							  "BAMCAQYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUK9BpR5R2Cf70a40uQKb3\n"
							  "R01/CF4wHwYDVR0jBBgwFoAUK9BpR5R2Cf70a40uQKb3R01/CF4wggERBgNVHSAE\n"
							  "ggEIMIIBBDCCAQAGCSqGSIb3Y2QFATCB8jAqBggrBgEFBQcCARYeaHR0cHM6Ly93\n"
							  "d3cuYXBwbGUuY29tL2FwcGxlY2EvMIHDBggrBgEFBQcCAjCBthqBs1JlbGlhbmNl\n"
							  "IG9uIHRoaXMgY2VydGlmaWNhdGUgYnkgYW55IHBhcnR5IGFzc3VtZXMgYWNjZXB0\n"
							  "YW5jZSBvZiB0aGUgdGhlbiBhcHBsaWNhYmxlIHN0YW5kYXJkIHRlcm1zIGFuZCBj\n"
							  "b25kaXRpb25zIG9mIHVzZSwgY2VydGlmaWNhdGUgcG9saWN5IGFuZCBjZXJ0aWZp\n"
							  "Y2F0aW9uIHByYWN0aWNlIHN0YXRlbWVudHMuMA0GCSqGSIb3DQEBBQUAA4IBAQBc\n"
							  "NplMLXi37Yyb3PN3m/J20ncwT8EfhYOFG5k9RzfyqZtAjizUsZAS2L70c5vu0mQP\n"
							  "y3lPNNiiPvl4/2vIB+x9OYOLUyDTOMSxv5pPCmv/K/xZpwUJfBdAVhEedNO3iyM7\n"
							  "R6PVbyTi69G3cN8PReEnyvFteO3ntRcXqNx+IjXKJdXZD9Zr1KIkIxH3oayPc4Fg\n"
							  "xhtbCS+SsvhESPBgOJ4V9T0mZyCKM2r3DYLP3uujL/lTaltkwGMzd/c6ByxW69oP\n"
							  "IQ7aunMZT7XZNn/Bh1XZp5m5MkL72NVxnn6hUrcbvZNCJBIqxw8dtk2cXmPIS4AX\n"
							  "UKqK1drk/NAJBzewdXUh\n"
							  "-----END CERTIFICATE-----\n";

bool CMSError()
{
	ERR_print_errors_fp(stdout);
	return false;
}

bool GenerateCMS(X509 *scert, EVP_PKEY *spkey, const string &strCDHashData, const string &strCDHashesPlist, string &strCMSOutput)
{
	if (!scert || !spkey)
	{
		return CMSError();
	}

	BIO *bother1 = BIO_new_mem_buf(appleDevCACert, strlen(appleDevCACert));
	BIO *bother2 = BIO_new_mem_buf(appleRootCACert, strlen(appleRootCACert));
	if (!bother1 || !bother2)
	{
		return CMSError();
	}

	X509 *ocert1 = PEM_read_bio_X509(bother1, NULL, 0, NULL);
	X509 *ocert2 = PEM_read_bio_X509(bother2, NULL, 0, NULL);
	if (!ocert1 || !ocert2)
	{
		return CMSError();
	}

	STACK_OF(X509) *otherCerts = sk_X509_new_null();
	if (!otherCerts)
	{
		return CMSError();
	}

	if (!sk_X509_push(otherCerts, ocert1))
	{
		return CMSError();
	}

	if (!sk_X509_push(otherCerts, ocert2))
	{
		return CMSError();
	}

	BIO *in = BIO_new_mem_buf(strCDHashData.c_str(), strCDHashData.size());
	if (!in)
	{
		return CMSError();
	}

	int nFlags = CMS_DETACHED | CMS_NOSMIMECAP | CMS_BINARY | CMS_PARTIAL;

	CMS_ContentInfo *cms = CMS_sign(NULL, NULL, otherCerts, in, nFlags);
	if (!cms)
	{
		return CMSError();
	}

	CMS_SignerInfo *si = CMS_add1_signer(cms, scert, spkey, NULL, nFlags);
	if (!si)
	{
		return CMSError();
	}

	ASN1_OBJECT *obj = OBJ_txt2obj("1.2.840.113635.100.9.1", 1);
	if (!obj)
	{
		return CMSError();
	}

	if (!CMS_signed_add1_attr_by_OBJ(si, obj, 0x4, strCDHashesPlist.c_str(), strCDHashesPlist.size()))
	{
		return CMSError();
	}

	if (!CMS_final(cms, in, NULL, nFlags))
	{
		return CMSError();
	}

	BIO *out = BIO_new(BIO_s_mem());
	if (!out)
	{
		return CMSError();
	}

	//PEM_write_bio_CMS(out, cms);
	if (!i2d_CMS_bio(out, cms))
	{
		return CMSError();
	}

	BUF_MEM *bptr = NULL;
	BIO_get_mem_ptr(out, &bptr);
	if (!bptr)
	{
		return CMSError();
	}

	strCMSOutput.clear();
	strCMSOutput.append(bptr->data, bptr->length);
	return (!strCMSOutput.empty());
}

bool GenerateCMS(const string &strSignerCertData, const string &strSignerPKeyData, const string &strCDHashData, const string &strCDHashesPlist, string &strCMSOutput)
{
	BIO *bcert = BIO_new_mem_buf(strSignerCertData.c_str(), strSignerCertData.size());
	BIO *bpkey = BIO_new_mem_buf(strSignerPKeyData.c_str(), strSignerPKeyData.size());

	if (!bcert || !bpkey)
	{
		return CMSError();
	}

	X509 *scert = PEM_read_bio_X509(bcert, NULL, 0, NULL);
	EVP_PKEY *spkey = PEM_read_bio_PrivateKey(bpkey, NULL, 0, NULL);
	if (!scert || !spkey)
	{
		return CMSError();
	}

	return GenerateCMS(scert, spkey, strCDHashData, strCDHashesPlist, strCMSOutput);
}

bool GetCMSContent(const string &strCMSDataInput, string &strContentOutput)
{
	if (strCMSDataInput.empty())
	{
		return false;
	}

	BIO *in = BIO_new(BIO_s_mem());
	OPENSSL_assert((size_t)BIO_write(in, strCMSDataInput.data(), strCMSDataInput.size()) == strCMSDataInput.size());
	CMS_ContentInfo *cms = d2i_CMS_bio(in, NULL);
	if (!cms)
	{
		return CMSError();
	}

	ASN1_OCTET_STRING **pos = CMS_get0_content(cms);
	if (!pos)
	{
		return CMSError();
	}

	if (!(*pos))
	{
		return CMSError();
	}

	strContentOutput.clear();
	strContentOutput.append((const char *)(*pos)->data, (*pos)->length);
	return (!strContentOutput.empty());
}

bool GetCertSubjectCN(X509 *cert, string &strSubjectCN)
{
	if (!cert)
	{
		return CMSError();
	}

	X509_NAME *name = X509_get_subject_name(cert);

	int common_name_loc = X509_NAME_get_index_by_NID(name, NID_commonName, -1);
	if (common_name_loc < 0)
	{
		return CMSError();
	}

	X509_NAME_ENTRY *common_name_entry = X509_NAME_get_entry(name, common_name_loc);
	if (common_name_entry == NULL)
	{
		return CMSError();
	}

	ASN1_STRING *common_name_asn1 = X509_NAME_ENTRY_get_data(common_name_entry);
	if (common_name_asn1 == NULL)
	{
		return CMSError();
	}

	strSubjectCN.clear();
	strSubjectCN.append((const char *)common_name_asn1->data, common_name_asn1->length);
	return (!strSubjectCN.empty());
}

bool GetCertSubjectCN(const string &strCertData, string &strSubjectCN)
{
	if (strCertData.empty())
	{
		return false;
	}

	BIO *bcert = BIO_new_mem_buf(strCertData.c_str(), strCertData.size());
	if (!bcert)
	{
		return CMSError();
	}

	X509 *cert = PEM_read_bio_X509(bcert, NULL, 0, NULL);
	if (!cert)
	{
		return CMSError();
	}

	return GetCertSubjectCN(cert, strSubjectCN);
}

void ParseCertSubject(const string &strSubject, JValue &jvSubject)
{
	vector<string> arrNodes;
	StringSplit(strSubject, "/", arrNodes);
	for (size_t i = 0; i < arrNodes.size(); i++)
	{
		vector<string> arrLines;
		StringSplit(arrNodes[i], "=", arrLines);
		if (2 == arrLines.size())
		{
			jvSubject[arrLines[0]] = arrLines[1];
		}
	}
}

string ASN1_TIMEtoString(ASN1_TIME *time)
{
	BIO *out = BIO_new(BIO_s_mem());
	if (!out)
	{
		CMSError();
		return "";
	}

	ASN1_TIME_print(out, time);
	BUF_MEM *bptr = NULL;
	BIO_get_mem_ptr(out, &bptr);
	if (!bptr)
	{
		CMSError();
		return "";
	}
	string strTime;
	strTime.append(bptr->data, bptr->length);
	return strTime;
}

bool GetCertInfo(X509 *cert, JValue &jvCertInfo)
{
	if (!cert)
	{
		return CMSError();
	}

	jvCertInfo["Version"] = (int)X509_get_version(cert);

	ASN1_INTEGER *asn1_i = X509_get_serialNumber(cert);
	if (asn1_i)
	{
		BIGNUM *bignum = ASN1_INTEGER_to_BN(asn1_i, NULL);
		if (bignum)
		{
			jvCertInfo["SerialNumber"] = BN_bn2hex(bignum);
		}
	}

	jvCertInfo["SignatureAlgorithm"] = OBJ_nid2ln(X509_get_signature_nid(cert));

	EVP_PKEY *pubkey = X509_get_pubkey(cert);
	int type = EVP_PKEY_id(pubkey);
	jvCertInfo["PublicKey"]["Algorithm"] = OBJ_nid2ln(type);

	jvCertInfo["Validity"]["NotBefore"] = ASN1_TIMEtoString(X509_get_notBefore(cert));
	jvCertInfo["Validity"]["NotAfter"] = ASN1_TIMEtoString(X509_get_notAfter(cert));
	
	/*
	uint32_t ex_kusage = X509_get_key_usage(cert);
	if(KU_DATA_ENCIPHERMENT == (ex_kusage & KU_DATA_ENCIPHERMENT))
	{

	}
	else if(KU_DIGITAL_SIGNATURE == (ex_kusage & KU_DIGITAL_SIGNATURE))
	{

	}

	const STACK_OF(X509_EXTENSION)* exts = X509_get0_extensions(cert);
	for (int i = 0; i < sk_X509_EXTENSION_num(exts); i++) 
	{
		X509_EXTENSION* ext = sk_X509_EXTENSION_value(exts, i);

		ASN1_OBJECT* obj = X509_EXTENSION_get_object(ext);
		const char* sn = OBJ_nid2ln(OBJ_obj2nid(obj));
		if(0 == strcmp("UNDEF", sn) || 0 == strcmp("undefined", sn))
		{
			char buff[1024] = {0};
			OBJ_obj2txt(buff, 1024, obj, 0); // 0 means it will prefer a textual representation (if available) rather than the numerical one
			jvCertInfo["Extensions"].push_back(buff);
		}
		else
		{
			jvCertInfo["Extensions"].push_back(sn);
		}
		
		int bcritical = X509_EXTENSION_get_critical(ext);
	   	ASN1_OCTET_STRING* data = X509_EXTENSION_get_data(ext);
*/
	/*
        if (!X509V3_EXT_print(bp, ex, flag, indent + 4)) {
            BIO_printf(bp, "%*s", indent + 4, "");
            ASN1_STRING_print(bp, X509_EXTENSION_get_data(ex));
        }   
*/

	//	}

	string strIssuer = X509_NAME_oneline(X509_get_issuer_name(cert), NULL, 0);
	string strSubject = X509_NAME_oneline(X509_get_subject_name(cert), NULL, 0);

	ParseCertSubject(strIssuer, jvCertInfo["Issuer"]);
	ParseCertSubject(strSubject, jvCertInfo["Subject"]);

	return (!strIssuer.empty() && !strSubject.empty());
}

bool GetCMSInfo(uint8_t *pCMSData, uint32_t uCMSLength, JValue &jvOutput)
{
	BIO *in = BIO_new(BIO_s_mem());
	OPENSSL_assert((size_t)BIO_write(in, pCMSData, uCMSLength) == uCMSLength);
	CMS_ContentInfo *cms = d2i_CMS_bio(in, NULL);
	if (!cms)
	{
		return CMSError();
	}

	int detached = CMS_is_detached(cms);
	jvOutput["detached"] = detached;

	const ASN1_OBJECT *obj = CMS_get0_type(cms);
	const char *sn = OBJ_nid2ln(OBJ_obj2nid(obj));
	jvOutput["contentType"] = sn;

	ASN1_OCTET_STRING **pos = CMS_get0_content(cms);
	if (pos)
	{
		if ((*pos))
		{
			ZBase64 b64;
			jvOutput["content"] = b64.Encode((const char *)(*pos)->data, (*pos)->length);
		}
	}

	STACK_OF(X509) *certs = CMS_get1_certs(cms);
	for (int i = 0; i < sk_X509_num(certs); i++)
	{
		JValue jvCertInfo;
		if (GetCertInfo(sk_X509_value(certs, i), jvCertInfo))
		{
			jvOutput["certs"].push_back(jvCertInfo);
		}
	}

	STACK_OF(CMS_SignerInfo) *sis = CMS_get0_SignerInfos(cms);
	for (int i = 0; i < sk_CMS_SignerInfo_num(sis); i++)
	{
		CMS_SignerInfo* si = sk_CMS_SignerInfo_value(sis, i);
		//int CMS_SignerInfo_get0_signer_id(CMS_SignerInfo *si, ASN1_OCTET_STRING **keyid, X509_NAME **issuer, ASN1_INTEGER **sno);

		int nSignedAttsCount = CMS_signed_get_attr_count(si);
		for(int j = 0; j < nSignedAttsCount; j++)
		{
			X509_ATTRIBUTE * attr = CMS_signed_get_attr(si, j);
			if(!attr)
			{
				continue;
			}
			int nCount = X509_ATTRIBUTE_count(attr);
			if(nCount <= 0)
			{
				continue;
			}

			ASN1_OBJECT * obj = X509_ATTRIBUTE_get0_object(attr);
			if(!obj)
			{
				continue;
			}

			char txtobj[128] = {0};
			OBJ_obj2txt(txtobj, 128, obj, 1);

			if(0 == strcmp("1.2.840.113549.1.9.3", txtobj))
			{//V_ASN1_OBJECT
				ASN1_TYPE *av = X509_ATTRIBUTE_get0_type(attr, 0);
				if(NULL != av)
				{
					jvOutput["attrs"]["ContentType"]["obj"] = txtobj;
					jvOutput["attrs"]["ContentType"]["data"] = OBJ_nid2ln(OBJ_obj2nid(av->value.object));
				}
			}
			else if(0 == strcmp("1.2.840.113549.1.9.4", txtobj))
			{//V_ASN1_OCTET_STRING
				ASN1_TYPE *av = X509_ATTRIBUTE_get0_type(attr, 0);
				if(NULL != av)
				{
					string strSHASum;
					char buf[16] = {0};
					for (int m = 0; m < av->value.octet_string->length; m++)
					{
						sprintf(buf, "%02x", (uint8_t)av->value.octet_string->data[m]);
						strSHASum += buf;
					}
					jvOutput["attrs"]["MessageDigest"]["obj"] = txtobj;
					jvOutput["attrs"]["MessageDigest"]["data"] = strSHASum;
				}
			}
			else if(0 == strcmp("1.2.840.113549.1.9.5", txtobj))
			{//V_ASN1_UTCTIME
				ASN1_TYPE *av = X509_ATTRIBUTE_get0_type(attr, 0);
				if(NULL != av)
				{
					BIO *mem = BIO_new(BIO_s_mem());
					ASN1_UTCTIME_print(mem, av->value.utctime);
					BUF_MEM *bptr = NULL;
					BIO_get_mem_ptr(mem, &bptr);
					BIO_set_close(mem, BIO_NOCLOSE);
					string strTime;
					strTime.append(bptr->data, bptr->length);
					BIO_free_all(mem);

					jvOutput["attrs"]["SigningTime"]["obj"] = txtobj;
					jvOutput["attrs"]["SigningTime"]["data"] = strTime;
				}
			}
			else if(0 == strcmp("1.2.840.113635.100.9.2", txtobj))
			{//V_ASN1_SEQUENCE
				jvOutput["attrs"]["CDHashes2"]["obj"] =  txtobj;
				for(int m = 0; m < nCount; m++)
				{
					ASN1_TYPE *av = X509_ATTRIBUTE_get0_type(attr, m);
					if(NULL != av)
					{
						ASN1_STRING *s = av->value.sequence;
						
						BIO *mem = BIO_new(BIO_s_mem());

						ASN1_parse_dump(mem, s->data, s->length, 2, 0);
						BUF_MEM *bptr = NULL;
						BIO_get_mem_ptr(mem, &bptr);
						BIO_set_close(mem, BIO_NOCLOSE);
						string strData;
						strData.append(bptr->data, bptr->length);
						BIO_free_all(mem);


						string strSHASum;
						size_t pos1 = strData.find("[HEX DUMP]:");
						if(string::npos != pos1)
						{
							size_t pos2 = strData.find("\n", pos1);
							if(string::npos != pos2)
							{
								strSHASum = strData.substr(pos1 + 11, pos2 - pos1 - 11);
							}
						}
						transform(strSHASum.begin(), strSHASum.end(), strSHASum.begin(), ::tolower);
						jvOutput["attrs"]["CDHashes2"]["data"].push_back(strSHASum);
					}
				}
			}
			else if(0 == strcmp("1.2.840.113635.100.9.1", txtobj))
			{//V_ASN1_OCTET_STRING
				ASN1_TYPE *av = X509_ATTRIBUTE_get0_type(attr, 0);
				if(NULL != av)
				{
					string strPList;
					strPList.append((const char*)av->value.octet_string->data, av->value.octet_string->length);
					jvOutput["attrs"]["CDHashes"]["obj"] =  txtobj;
					jvOutput["attrs"]["CDHashes"]["data"] =  strPList;
				}
			}
			else
			{
				ASN1_TYPE *av = X509_ATTRIBUTE_get0_type(attr, 0);
				if(NULL != av)
				{
					JValue jvAttr;
					jvAttr["obj"] = txtobj;
					jvAttr["name"] = OBJ_nid2ln(OBJ_obj2nid(obj));
					jvAttr["type"] = av->type;
					jvAttr["count"] = nCount;
					jvOutput["attrs"]["unknown"].push_back(jvAttr);
				}
			}
		}
	}

	return true;
}

ZSignAsset::ZSignAsset()
{
	m_evpPkey = NULL;
	m_x509Cert = NULL;
}

bool ZSignAsset::Init(const string &strSignerCertFile, const string &strSignerPKeyFile, const string &strProvisionFile, const string &strEntitlementsFile, const string &strPassword)
{
	ReadFile(strProvisionFile.c_str(), m_strProvisionData);
	ReadFile(strEntitlementsFile.c_str(), m_strEntitlementsData);
	if (m_strProvisionData.empty())
	{
		ZLog::Error(">>> Can't Find Provision File!\n");
		return false;
	}

	JValue jvProv;
	string strProvContent;
	if (GetCMSContent(m_strProvisionData, strProvContent))
	{
		if (jvProv.readPList(strProvContent))
		{
			m_strTeamId = jvProv["TeamIdentifier"][0].asCString();
			if (m_strEntitlementsData.empty())
			{
				jvProv["Entitlements"].writePList(m_strEntitlementsData);
			}
		}
	}

	if (m_strTeamId.empty())
	{
		ZLog::Error(">>> Can't Find TeamId!\n");
		return false;
	}

	X509 *x509Cert = NULL;
	EVP_PKEY *evpPkey = NULL;
	BIO *bioPKey = BIO_new_file(strSignerPKeyFile.c_str(), "r");
	if (NULL != bioPKey)
	{
		evpPkey = PEM_read_bio_PrivateKey(bioPKey, NULL, NULL, (void *)strPassword.c_str());
		if (NULL == evpPkey)
		{
			BIO_reset(bioPKey);
			evpPkey = d2i_PrivateKey_bio(bioPKey, NULL);
			if (NULL == evpPkey)
			{
				BIO_reset(bioPKey);
				PKCS12 *p12 = d2i_PKCS12_bio(bioPKey, NULL);
				if (NULL != p12)
				{
					if (0 == PKCS12_parse(p12, strPassword.c_str(), &evpPkey, &x509Cert, NULL))
					{
						CMSError();
					}
					PKCS12_free(p12);
				}
			}
		}
		BIO_free(bioPKey);
	}

	if (NULL == evpPkey)
	{
		ZLog::Error(">>> Can't Load P12 or PrivateKey File! Please Input The Correct File And Password!\n");
		return false;
	}

	if (NULL == x509Cert && !strSignerCertFile.empty())
	{
		BIO *bioCert = BIO_new_file(strSignerCertFile.c_str(), "r");
		if (NULL != bioCert)
		{
			x509Cert = PEM_read_bio_X509(bioCert, NULL, 0, NULL);
			if (NULL == x509Cert)
			{
				BIO_reset(bioCert);
				x509Cert = d2i_X509_bio(bioCert, NULL);
			}
			BIO_free(bioCert);
		}
	}

	if (NULL != x509Cert)
	{
		if (!X509_check_private_key(x509Cert, evpPkey))
		{
			X509_free(x509Cert);
			x509Cert = NULL;
		}
	}

	if (NULL == x509Cert)
	{
		for (size_t i = 0; i < jvProv["DeveloperCertificates"].size(); i++)
		{
			string strCertData = jvProv["DeveloperCertificates"][i].asData();
			BIO *bioCert = BIO_new_mem_buf(strCertData.c_str(), strCertData.size());
			if (NULL != bioCert)
			{
				x509Cert = d2i_X509_bio(bioCert, NULL);
				if (NULL != x509Cert)
				{
					if (X509_check_private_key(x509Cert, evpPkey))
					{
						break;
					}
					X509_free(x509Cert);
					x509Cert = NULL;
				}
				BIO_free(bioCert);
			}
		}
	}

	if (NULL == x509Cert)
	{
		ZLog::Error(">>> Can't Find Paired Certificate And PrivateKey!\n");
		return false;
	}

	if (!GetCertSubjectCN(x509Cert, m_strSubjectCN))
	{
		ZLog::Error(">>> Can't Find Paired Certificate Subject Common Name!\n");
		return false;
	}

	m_evpPkey = evpPkey;
	m_x509Cert = x509Cert;
	return true;
}

bool ZSignAsset::GenerateCMS(const string &strCDHashData, const string &strCDHashesPlist, string &strCMSOutput)
{
	return ::GenerateCMS((X509 *)m_x509Cert, (EVP_PKEY *)m_evpPkey, strCDHashData, strCDHashesPlist, strCMSOutput);
}