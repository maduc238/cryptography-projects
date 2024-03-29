#!/usr/bin/env python

"""Decode an X509 certificate.

"""

from __future__ import print_function
import os
import base64
import asn1tools


SCRIPT_DIR = os.path.dirname(os.path.realpath(__file__))
RFC5280_ASN_PATH = 'rfc5280.asn'

CERTIFICATE_PEM = '''\
-----BEGIN CERTIFICATE-----
MIIDCjCCAfICCQCoXfSQydXTczANBgkqhkiG9w0BAQsFADBHMQswCQYDVQQGEwJT
RTETMBEGA1UECAwKU29tZS1TdGF0ZTEOMAwGA1UECgwFU2ltYmExEzARBgNVBAMM
CmZvb2Jhci5vcmcwHhcNMTcwMTA3MTgxOTQ3WhcNMTgwMTA3MTgxOTQ3WjBHMQsw
CQYDVQQGEwJTRTETMBEGA1UECAwKU29tZS1TdGF0ZTEOMAwGA1UECgwFU2ltYmEx
EzARBgNVBAMMCmZvb2Jhci5vcmcwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEK
AoIBAQDugoc9amynDB/ODJzh2aqB6cnubSJEPlgB87jNG0akcbmUFt6BhPhMPSoP
esHWl5OWscoga0cKrPURmMVVhfaeZLQGmrv5N4/liVlwae1n0gUEruX4d6MqSSDW
3C/WKjCn647udZwyzCvyrvPOq0qAzaxR4EFRdwjSEPO5sDw2zxeTjGW2WxaH9PEu
C8vaNqTsLYl3YBkR3zVCbuQXTQhStsv3gT4Yhz2wJpY0yyWyDiaBkGKpdxJQiNAd
x5JKSqtRshlYZM3+cdKLywNoYUnezp6Wm4mzz09TCFv+esJ0h7/6pMdVjhxLsAg5
ZbZyrtNIapN07AjIJS4qjkJ/HUC3AgMBAAEwDQYJKoZIhvcNAQELBQADggEBAGHS
U5AvDESzTNoak5HHx166bp5+bvECvJg45GMDGhqKeFoOlR34u2b+wyTm/piz3nYJ
12kn+BbG/PwGfndL0kZYRz46fY8Rf+MxCFfcglweDJhA6ULNpera11OC35Q/lKn5
M6w6eQkZMB4VqwigvDGHGpXRTIhJHHoR2VFBFGoPTLrXilChUpiXi9DmuYXJ/19x
sxOVwvvO/m/6g68G+uZYUoCsQsKllM2fgdNLTzbYvnFtsq5QnZS8m7CoZgEy2c3m
VrrPsfwmyhwejDawjz2epSLNXaaDeSz4g1cQes+oehaA1IwFfKhb9tdiWUm48VuU
oaFZ8e1HofYUy+65pws=
-----END CERTIFICATE-----\
'''

certificate = CERTIFICATE_PEM.splitlines()[1:-1]
certificate = ''.join(certificate)
certificate = base64.b64decode(certificate)

rfc5280_der = asn1tools.compile_files(RFC5280_ASN_PATH, codec='der')
rfc5280_gser = asn1tools.compile_files(RFC5280_ASN_PATH, codec='gser')

decoded = rfc5280_der.decode('Certificate', certificate)
encoded = rfc5280_gser.encode('Certificate', decoded, indent=4)

print(encoded.decode('ascii'))