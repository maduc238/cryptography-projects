import base64
import asn1tools
import rsa
from asn1tools.codecs import restricted_utc_time_to_datetime as ut2dt
from asn1 import ASN1


RFC5280_ASN_PATH = 'rfc5280.asn'
foo = asn1tools.compile_files(RFC5280_ASN_PATH)

############### CA key #################
CApub  = rsa.PublicKey(7565405484170034754798823201531508951647626123274235527309334239020114596630701625473847384118469818457769738587018452263277539686253548751055736519676817, 65537)
CApriv = rsa.PrivateKey(7565405484170034754798823201531508951647626123274235527309334239020114596630701625473847384118469818457769738587018452263277539686253548751055736519676817, 65537, 6129482588437989310012785363305004231420200068566082640784152912453881693569558207876842096285280408270208375044068089851882029784925566573024013327187177, 5658007054291830605098027319389505840137258235008723905257557849173760699563756703, 1337114890733719418786433202844217816323769187066936935471372501993701839)
############### CA key #################

############### certificate data field #################
# Version
_version = 'v1'
# Serial number
_serialNumber = 3578
# Issuer
class _issuer:
    _countryName = ASN1.EncodeASN1('VN')
    _stateOrProvinceName = ASN1.EncodeASN1('Hanoi')
    _localityName = ASN1.EncodeASN1('Hai Ba Trung')
    _organizationName = ASN1.EncodeASN1('MaDuc238')
    _organizationalUnitName = ASN1.EncodeASN1('Tai nang K64')
    _commonName = ASN1.EncodeASN1('Ma Duc')
    _emailAddress = ASN1.EncodeASN1('mavietduc@gmail.com')
# Validity
class _validity:
    _from = ut2dt('120822052654Z')
    _to = ut2dt('230821052654Z')
# Subject
class _subject:
    _countryName = ASN1.EncodeASN1('VN')
    _stateOrProvinceName = ASN1.EncodeASN1('Hanoi')
    _organizationName = ASN1.EncodeASN1('DHBK')
    _commonName = ASN1.EncodeASN1('User1')
# Public Key
class _publicKeyInfo:
    _algorithm = '1.2.840.113549.1.1.1'     # RSA
    _parameters = b'\x05\x00'
    _publickey = (b'0H\x02A\x00\x9b\xfcf\x90y\x84B\xbb'
                    b'\xab\x13\xfd+{\xf8\xde\x15\x12\xe5'
                    b'\xf1\x93\xe3\x06\x8a{\xb8\xb1\xe1'
                    b'\x9e&\xbb\x95\x01\xbf\xe70\xedd\x85'
                    b'\x02\xdd\x15i\xa84\xb0\x06\xec?5<'
                    b'\x1e\x1b+\x8f\xfa\x8f\x00\x1b\xdf'
                    b'\x07\xc6\xacS\x07\x02\x03\x01\x00'
                    b'\x01',592)
# Signature
class _signature:
    _algorithm = '1.2.840.113549.1.1.5'        # sha1RSA
    _parameters = b'\x05\x00'                  # sha1
############### end of modified #################

tbsCertificate = {
        'version': _version,
        'serialNumber': _serialNumber,
        'signature': {
            'algorithm': _signature._algorithm,
            'parameters': _signature._parameters
        },
        'issuer': (
            'rdnSequence',
            [
                [{'type': '2.5.4.6',
                    'value': _issuer._countryName}],
                [{'type': '2.5.4.8',
                    'value': _issuer._stateOrProvinceName}],
                [{'type': '2.5.4.7',
                    'value': _issuer._localityName}],
                [{'type': '2.5.4.10',
                    'value': _issuer._organizationName}],
                [{'type': '2.5.4.11',
                    'value': _issuer._organizationalUnitName}],
                [{'type': '2.5.4.3',
                    'value': _issuer._commonName}],
                [{'type': '1.2.840.113549.1.9.1',
                    'value': _issuer._emailAddress}]
            ]
        ),
        'validity': {
            'notAfter': ('utcTime', _validity._to),
            'notBefore': ('utcTime', _validity._from)
        },
        'subject': (
            'rdnSequence',
            [
                [{'type': '2.5.4.6',
                    'value': _subject._countryName}],
                [{'type': '2.5.4.8',
                    'value': _subject._stateOrProvinceName}],
                [{'type': '2.5.4.10',
                    'value': _subject._organizationName}],
                [{'type': '2.5.4.3',
                    'value': _subject._commonName}]
            ]
        ),
        'subjectPublicKeyInfo': {
            'algorithm': {
                'algorithm': _publicKeyInfo._algorithm,
                'parameters': _publicKeyInfo._parameters
            },
            'subjectPublicKey': _publicKeyInfo._publickey
        }
    }

encoded_TBSCertificate = foo.encode('TBSCertificate', tbsCertificate)
signature = rsa.sign(encoded_TBSCertificate, CApriv, 'SHA-1')

data = {
    'tbsCertificate': tbsCertificate,
    'signatureAlgorithm': {
        'algorithm': _signature._algorithm,
        'parameters': _signature._parameters
    },
    'signature': (signature, len(signature)*8)
}

encoded = foo.encode('Certificate', data)
print('-----BEGIN CERTIFICATE-----')
cer_data = base64.b64encode(encoded).decode()
for i in range(len(cer_data)//64 + 1):
    print(cer_data[64*i:64*(i+1)])
print('-----END CERTIFICATE-----')