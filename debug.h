#define HRNAME(hr) printf("%s:%d - %s\n", __FILE__,__LINE__, HRESULTExplain((hr)) )

const char *debugstr_guid( const struct _GUID *id )
{
    static char str[100];

    if (!id)
        return "(null)";
    sprintf( str, "{%08lx-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
             id->Data1, id->Data2, id->Data3,
             id->Data4[0], id->Data4[1], id->Data4[2], id->Data4[3],
             id->Data4[4], id->Data4[5], id->Data4[6], id->Data4[7] );
    return str;

}

const char* HRESULTExplain(HRESULT hr)
{

    static HRESULT __keys[] = { S_OK, E_NOTIMPL, E_NOINTERFACE, E_POINTER,
        E_ABORT, E_FAIL, E_UNEXPECTED, E_ACCESSDENIED, E_HANDLE, E_OUTOFMEMORY,
        E_INVALIDARG
    };

    static const char* __values[] = { "Operation successful", "Not implemented",
        "Interface not supported", "Invalid Pointer", "Aborted", "Unspecified failure",
        "Unexpected failure", "Access denied", "Invalid handle", "Out of Memory",
        "Invalid argumments"
    };

    for (int i=0; i < sizeof(__keys); i++ ) {
        if (hr == __keys[i])
            return __values[i];
    }
}

const char* to_binary(unsigned int hex)
{
    static char buffer[256];
    int index = 39;
    for (int i=0; i < 32; i++) {
        buffer[index] = 48 + (hex % 2);
        index--;
        if ((index % 5)==0) {
            buffer[index] = ' ';
            index--;
        }
        hex /= 2;
    }
    buffer[40] = '\0';
    return buffer;
}
