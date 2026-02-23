#include <Windows.h>
#include <iostream>
#include <evntprov.h>

#pragma comment(lib, "advapi32.lib")

REGHANDLE g_EtwHandle = 0;

// Descriptor del evento
EVENT_DESCRIPTOR g_EventRestar =
{
    1,    // Id
    0,    // Version
    0,    // Channel
    4,    // Level (Information)
    0,    // Opcode
    0,    // Task
    0     // Keyword
};

// GUID del provider
static const GUID my_guid =
{ 0x6a282996,0x820b,0x42f3,{0x87,0x25,0x57,0x3f,0xd3,0x09,0xcd,0x0a} };

int main()
{
    ULONG error;

    std::cout << "[+] Iniciando Provider\n";

    // Registrar provider
    error = EventRegister(&my_guid, NULL, NULL, &g_EtwHandle);

    if (error != ERROR_SUCCESS)
    {
        std::cout << "[-] EventRegister error: " << error << "\n";
        return -1;
    }

    std::cout << "[+] Provider registrado\n";

    // Datos a enviar
    DWORD pid = GetCurrentProcessId();

    WCHAR text[] = L"Enviado desde el provider";

    EVENT_DATA_DESCRIPTOR data[2];

    // Campo 1: PID
    EventDataDescCreate(
        &data[0],
        &pid,
        sizeof(pid)
    );

    // Campo 2: Texto
    EventDataDescCreate(
        &data[1],
        text,
        (ULONG)((wcslen(text) + 1) * sizeof(WCHAR))
    );

    // Enviar evento
    error = EventWrite(
        g_EtwHandle,
        &g_EventRestar,
        2,
        data
    );

    if (error != ERROR_SUCCESS)
    {
        std::cout << "[-] EventWrite error: " << error << "\n";
    }
    else
    {
        std::cout << "[+] Evento enviado correctamente\n";
    }

    // Mantener vivo el proceso
    std::cout << "[+] Presiona Enter para salir...\n";
    std::cin.get();

    // Desregistrar
    error = EventUnregister(g_EtwHandle);

    if (error != ERROR_SUCCESS)
    {
        std::cout << "[-] EventUnregister error: " << error << "\n";
    }

    return 0;
}