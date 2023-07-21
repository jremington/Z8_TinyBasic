/** PC Driver serial port program for USB-RS232 UART adapters
 * This code uploads a Tiny Basic program file to the serial port on a Z8, 2400 Baud
 * :REM statements are stripped out to save memory on the Z8
 *
 *  Serial port code was based on on Pololu's MaestroSerialExampleCWindows:
 *  Example program for sending and receiving bytes from the Maestro over a serial port
 *  in C on Windows.
 *
 *  All the Windows functions called by the program are documented on MSDN:
 *  http://msdn.microsoft.com/
 *
 *  The error codes that this program may output are documented on MSDN:
 *  http://msdn.microsoft.com/en-us/library/ms681381%28v=vs.85%29.aspx
 *
 *
 *  For an advanced guide to serial port communication in Windows, see:
 *  http://msdn.microsoft.com/en-us/library/ms810467
 *  S. James Remington May & Nov 2012
*/

#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <string.h>
#include <conio.h>

//global serial port handle

HANDLE SerPrt = INVALID_HANDLE_VALUE;

/** Opens a handle to a serial port in Windows using CreateFile.
 * portName: The name of the port.
 * baudRate: The baud rate in bits per second.
 * Returns INVALID_HANDLE_VALUE if it fails.  Otherwise returns a handle to the port.
 *   Examples: "COM4", "\\\\.\\USBSER000", "USB#VID_1FFB&PID_0089&MI_04#6&3ad40bf600004#
 */

HANDLE openPort(const char * portName, unsigned int baudRate)
{
	HANDLE port;
	DCB commState;
	BOOL success;
	COMMTIMEOUTS timeouts;

	/* Open the serial port. */
	port = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (port == INVALID_HANDLE_VALUE)
	{
		switch(GetLastError())
		{
		case ERROR_ACCESS_DENIED:
			fprintf(stderr, "Error: Access denied.  Try closing all other programs that are using the device.\n");
			break;
		case ERROR_FILE_NOT_FOUND:
			fprintf(stderr, "Error: Serial port not found.  "
				"Make sure that \"%s\" is the right port name.  "
				"Try closing all programs using the device and unplugging the "
				"device, or try rebooting.\n", portName);
			break;
		default:
			fprintf(stderr, "Error: Unable to open serial port.  Error code 0x%lx.\n", GetLastError());
			break;
		}
		return INVALID_HANDLE_VALUE;
	}

	/* Set the timeouts. */
	success = GetCommTimeouts(port, &timeouts);
	if (!success)
	{
		fprintf(stderr, "Error: Unable to get comm timeouts.  Error code 0x%lx.\n", GetLastError());
		CloseHandle(port);
		return INVALID_HANDLE_VALUE;
	}
	timeouts.ReadIntervalTimeout = 1000;
	timeouts.ReadTotalTimeoutConstant = 1000;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 1000;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	success = SetCommTimeouts(port, &timeouts);
	if (!success)
	{
		fprintf(stderr, "Error: Unable to set comm timeouts.  Error code 0x%lx.\n", GetLastError());
		CloseHandle(port);
		return INVALID_HANDLE_VALUE;
	}

	/* Set the baud rate. */
	success = GetCommState(port, &commState);
	if (!success)
	{
		fprintf(stderr, "Error: Unable to get comm state.  Error code 0x%lx.\n", GetLastError());
		CloseHandle(port);
		return INVALID_HANDLE_VALUE;
	}
	commState.BaudRate = baudRate;
	success = SetCommState(port, &commState);
	if (!success)
	{
		fprintf(stderr, "Error: Unable to set comm state.  Error code 0x%lx.\n", GetLastError());
		CloseHandle(port);
		return INVALID_HANDLE_VALUE;
	}

	/* Flush out any bytes received from the device earlier. */
	success = FlushFileBuffers(port);
	if (!success)
	{
		fprintf(stderr, "Error: Unable to flush port buffers.  Error code 0x%lx.\n", GetLastError());
		CloseHandle(port);
		return INVALID_HANDLE_VALUE;
	}

	return port;
}

// For efficiency, SerialPutc is used only to transfer displayable character strings.
// Overall performance could be further improved by copying & writing the entire string.

void SerialPutc(HANDLE port, unsigned char t)
{
	DWORD bytesTransferred;
	BOOL success;
    unsigned char command[1];
    command[0]=t;

	// Send the character to the device.
	success = WriteFile(SerPrt, command, 1, &bytesTransferred, NULL);
	if (!success)
	{
		fprintf(stderr, "Error: Unable to write character to serial port.  Error code 0x%lx.", GetLastError());
		return;
	}
	if (1 != bytesTransferred)
	{
		fprintf(stderr, "Error: Expected to write one byte but wrote %ld.", bytesTransferred);
		return;
	}
}


/** Main Program */

int main(int argc, char * argv[])
{

	char * portName;
	int baudRate, i;

	/* portName should be the name of the Scope Clock Port (e.g. "COM4")
	 * as shown in the computer's Device Manager.
	 * Alternatively you can use \\.\USBSER000 to specify the first virtual COM
	 * port that uses the usbser.sys driver.
	 */
//	portName = "\\\\.\\USBSER000";  // Each double slash in this source code represents one slash in the actual name.
	portName = "\\\\.\\COM6";  // Each double slash in this source code represents one slash in the actual name.
	baudRate = 2400;
    char filename[80]={0}, buf[101]={0};
    int buflen;
    FILE *fpin;

    int num_char=0;

	/* Open the serial port. */
	SerPrt = openPort(portName, baudRate);
	if (SerPrt == INVALID_HANDLE_VALUE) return -1;

	int first=1;  //first line dump

    printf("\nInput file? ");
    scanf("%s",&filename);
    fpin = fopen (filename, "r");
    if (fpin == NULL) {
    printf("open failure %s code:%d\n",filename,errno);
   }


 while(fgets(buf, 100, fpin) != NULL) {

        buflen=strlen(buf);

//dump first line in hex
    if(first) {
    printf("First line, buflen %d\n",buflen);
    for(i=0; i< buflen; i++) printf("%02X ",buf[i]);
    printf("\n");
    first=0;
   }
// strip out final ":REM
    char * pch;
    pch = strstr (buf,":REM");
    if (pch != NULL) {
      buflen = pch-buf;
      buf[buflen++]=0x0A;
      buf[buflen]=0;
      }
    printf("%s",buf);

    for(i=0; i< buflen; i++) {
    char c=buf[i];
    if (c != 0x0D && c != 0x0A) {
         SerialPutc(&SerPrt,c); //skip line endings
         Sleep(2);
         num_char++;
        }
    }
    SerialPutc(&SerPrt,0x0D); //send carriage return
    num_char++;
    Sleep(50); //30 ms, give micro time to process the line
 }
    /* Close the serial port so other programs can use it.
	 * Alternatively, you can just terminate the process (return from main). */
	 printf("\n %d chars transmitted. Done uploading.\n", num_char);

	CloseHandle(SerPrt);

	return 0;
}
