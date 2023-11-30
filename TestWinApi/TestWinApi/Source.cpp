#include "framework.h"
#include "resource.h"
#include <fstream>
#include <CommDlg.h>

using namespace std;

#define MAX_LOADSTRING 100

//INT_PTR CALLBACK fVentana1(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK fLogin(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK fDoctores(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK fCitas(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK fMenu(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK fEventos(HWND, UINT, WPARAM, LPARAM);

WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

struct nodo_doctores {
	char nombre[100];
	char usuario[20];
	char pass[20];
	char cedula[20];
	char foto[MAX_PATH];
	int  id;
	nodo_doctores* ant;
	nodo_doctores* sig;
};
nodo_doctores* aux = 0, * lista_doctores = 0, * ultimo_doctor = 0, * nuevo_doctor = 0, * doctor_seleccionado = 0;

struct nodo_mascotas {
	char fecha[100];
	char hora[100];
	char nombre[100];
	char telefono[100];
	char especie[100];
	char mascota[100];
	char motivo[100];
	char costo[18];
	char costoTotal[18];
	char estatus[100];
	char doctor_guardo[100];
	nodo_mascotas* ant;
	nodo_mascotas* sig;
};
nodo_mascotas* aux_mascotas = 0, * lista_mascotas = 0, * ultimo_mascota = 0, * nuevo_mascota = 0, * mascota_seleccionado = 0;


HINSTANCE hInst;                                // current instance

char arch_doctores[] = "doctores.dat";
char arch_mascotas[] = "mascotas.dat";
char nombrearchivo[MAX_PATH] = { 0 };
char mensaje[50] = { 0 };

int es_nuevo = 0;
int es_nuevo_mascota = 0;

void agregar_doctor(nodo_doctores* aux);
void leer_archivo_doctores(char* archivo);
void escribir_archivo_doctores(char* archivo);
void leer_doctores(HWND hwnd);
nodo_doctores* buscar_doctor_ant(char* valor);
int buscar_doctor(char* valor);
int login_doctor(char* valor);
void borrar_doctor(nodo_doctores* aux);
void cargar_imagen(HWND hDlg, char file[], int Pic);
void examinar(HWND hDlg);

void agregar_mascota(nodo_mascotas* aux);
void leer_archivo_mascotas(char* archivo);
void escribir_archivo_mascotas(char* archivo);
void borrar_mascota(nodo_mascotas* aux);
void leer_mascotas(HWND hwnd);
void leer_mascotas_fecha(HWND hwnd, char* valor);


int buscar_cita_nombre(char* valor);
int buscar_cita_hora(char* valor);

void agregar_eventos(HWND hwnd);
void agregar_especies(HWND hwnd);
void agregar_estatus(HWND hwnd);

HBITMAP foto;

//Pantalla de inicio de sesion
//Esta parte del codigo causa un aviso C28251
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, PSTR cmdLine, int cShow) {
	
	leer_archivo_doctores(arch_doctores);
	leer_archivo_mascotas(arch_mascotas);

	//HWND hDoctores1 = CreateDialog(hInst, MAKEINTRESOURCE(DLG_DOCTORES), NULL, fDoctores);
	HWND hLogin = CreateDialog(hInst, MAKEINTRESOURCE(DLG_LOGIN), NULL, fLogin);

	hInst = hInst;

	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	

	//ShowWindow(hDoctores1, cShow);
	ShowWindow(hLogin, cShow);

	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}

	escribir_archivo_doctores(arch_doctores);
	escribir_archivo_mascotas(arch_mascotas);
	return 0;

}

//Inicio de sesion
INT_PTR CALLBACK fLogin(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND hUsuario = GetDlgItem(hwnd, TXT_USUARIO_LOGIN);
	HWND hPass = GetDlgItem(hwnd, TXT_PASS_LOGIN);

	char usuario[20];
	char pass[20];

	switch (msg)
	{
		case WM_COMMAND:
		{
			if (LOWORD(wParam) == BTN_ACEPTAR_LOGIN && HIWORD(wParam) == BN_CLICKED)
			{
				GetWindowText(hUsuario, usuario, 20);
				GetWindowText(hPass, pass, 20);

				if (login_doctor(usuario) == 1)
				{
					if (strcmp(aux->pass, pass) == 0) {

						doctor_seleccionado = aux;
						DestroyWindow(hwnd);

						//Entra directamente a citas despues de iniciar sesion
						//HWND hCitas = CreateDialog(hInst, MAKEINTRESOURCE(DLG_CITAS), NULL, fCitas);
						HWND hMenu = CreateDialog(hInst, MAKEINTRESOURCE(DLG_MENU), NULL, fMenu);
						ShowWindow(hMenu, 1);
						//MessageBox(0, "Usuario Correcto", "Login", MB_ICONINFORMATION + MB_OK);
					}
					else
					{
						
						MessageBox(0, "Contraseña No Valida", "Login", MB_ICONINFORMATION + MB_OK);
					}

				}
				else
				{
					MessageBox(0, "Usuario No Valido", "Login", MB_ICONINFORMATION + MB_OK);
				}

			}

			if (LOWORD(wParam) == BTN_CANCELAR_LOGIN && HIWORD(wParam) == BN_CLICKED)
			{
				DestroyWindow(hwnd);
				PostQuitMessage(117);
			}

		}break;
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);

			break;
		}
		case WM_DESTROY:
			//PostQuitMessage(117);
			break;
	}
	return FALSE;

}


//Configuracion del menu
INT_PTR CALLBACK fMenu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HWND hNombre = GetDlgItem(hwnd, LBL_NOMBRE_MENU);

	switch (msg)
	{
		case WM_INITDIALOG:
		{
		
			SetWindowText(hNombre, doctor_seleccionado->nombre);
		}break;
		case WM_COMMAND:
		{
			if (LOWORD(wParam) == BTN_EVENTOS && HIWORD(wParam) == BN_CLICKED) //Eventos del mes 
			{
				HWND hEventos = CreateDialog(hInst, MAKEINTRESOURCE(DLG_EVENTOS), NULL, fEventos);
				ShowWindow(hEventos, 1);
			}

			if (LOWORD(wParam) == BTN_CITAS && HIWORD(wParam) == BN_CLICKED) //Realizar compra
			{
				HWND hCitas = CreateDialog(hInst, MAKEINTRESOURCE(DLG_CITAS), NULL, fCitas);
				ShowWindow(hCitas, 1);
			}

			if (LOWORD(wParam) == BTN_SALIR && HIWORD(wParam) == BN_CLICKED) //Mis compras
			{
				HWND hCitas = CreateDialog(hInst, MAKEINTRESOURCE(DLG_CITAS), NULL, fCitas);
				ShowWindow(hCitas, 1);
			}

		}break;
		case WM_CLOSE:
		{
			DestroyWindow(hwnd);

			break;
		}
		case WM_DESTROY:
			PostQuitMessage(117);
			break;
	}
	return FALSE;
}

INT_PTR CALLBACK fEventos(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch (msg)
	case WM_COMMAND:
	{
		if (LOWORD(wParam) == CMB_EVENTOS && HIWORD(wParam) == CBN_DROPDOWN)
		{
			agregar_eventos(hwnd);
		}
		if (LOWORD(wParam) == IDOK && HIWORD(wParam) == BN_CLICKED)
		{
			DestroyWindow(hwnd);
		}
		if (LOWORD(wParam) == IDC_SELECCION && HIWORD(wParam) == BN_CLICKED)
		{
			// Obtén el índice seleccionado en la lista desplegable
			int selectedIdx = SendDlgItemMessage(hwnd, CMB_EVENTOS, CB_GETCURSEL, 0, 0);
			if (selectedIdx != CB_ERR)
			{
				// Obten el texto del elemento seleccionado
				char selectedText[256];
				SendDlgItemMessage(hwnd, CMB_EVENTOS, CB_GETLBTEXT, selectedIdx, (LPARAM)selectedText);

				// Abre la ventana de citas y pasa el texto seleccionado
				HWND hCitas = CreateDialog(hInst, MAKEINTRESOURCE(DLG_CITAS), NULL, fCitas);
				HWND hMascota = GetDlgItem(hCitas, TXT_MASCOTA);
				SetWindowText(hMascota, selectedText);
				ShowWindow(hCitas, 1);
			}
		}
		break;
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		break;
	}
	case WM_DESTROY:
		DestroyWindow(hwnd);
		break;
	}
	return FALSE; 
}
void agregar_eventos(HWND hwnd)
{
	HWND hEventos = GetDlgItem(hwnd, CMB_EVENTOS);
		SendMessage(hEventos, CB_RESETCONTENT, 0, 0);
		SendMessage(hEventos, CB_ADDSTRING, 0, (LPARAM)"01 Dic        .:Los Indestructibles 3:.                             Pelicula ");
		SendMessage(hEventos, CB_ADDSTRING, 0, (LPARAM)"02 Dic        .:My Chemical Romance:.                          Concierto");
		SendMessage(hEventos, CB_ADDSTRING, 0, (LPARAM)"03 Dic        .:Le Monde:.                                              Pelicula ");
		SendMessage(hEventos, CB_ADDSTRING, 0, (LPARAM)"04 Dic        .:El Cascanueces:.                                     Teatro   ");
		SendMessage(hEventos, CB_ADDSTRING, 0, (LPARAM)"05 Dic        .:Avatar 3: El Camino del Agua:.               Pelicula ");
		SendMessage(hEventos, CB_ADDSTRING, 0, (LPARAM)"07 Dic        .:Black Panther:.                                         Pelicula ");
		SendMessage(hEventos, CB_ADDSTRING, 0, (LPARAM)"09 Dic        .:5 Seconds of Summer:.                            Concierto");
}

//Ventana de compra
INT_PTR CALLBACK fCitas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int idx = 0;
	char fecha[100];
	char hora[100];
	char nombre[100];
	char telefono[100];
	int cantidadBoletos = 0;
	int especie = 0;//Cantidad de tickets, se cambia a int
	char mascota[100]; //nombre del evento
	char motivo[100]; //no se usa
	int costo = 0;
	char estatus[100]; //Estatus de la compra
	float costoTotal = 0;

	HWND hFecha = GetDlgItem(hwnd, DTP_FECHA);
	HWND hHora = GetDlgItem(hwnd, DTP_HORA);
	HWND hNombre = GetDlgItem(hwnd, TXT_NOMBRE);
	HWND hTelefono = GetDlgItem(hwnd, TXT_TELEFONO);
	HWND hEspecie = GetDlgItem(hwnd, CMB_ESPECIE);
	HWND hMascota = GetDlgItem(hwnd, TXT_MASCOTA);
	HWND hMotivo = GetDlgItem(hwnd, TXT_MOTIVO);
	HWND hCosto = GetDlgItem(hwnd, TXT_COSTO);
	HWND hCostoTotal = GetDlgItem(hwnd, TXT_COSTO2);
	HWND hEstatus = GetDlgItem(hwnd, CMB_ESTATUS); 

	HWND hDoctor = GetDlgItem(hwnd, TXT_DOCTOR);//Perfil

	switch (msg)
	{
	case WM_INITDIALOG:
	{

		GetWindowText(hFecha, (LPTSTR)fecha, 100);


		leer_mascotas_fecha(hwnd, fecha);
		es_nuevo = 1;
		
		agregar_especies(hwnd);
		agregar_estatus(hwnd);
		SetWindowText(hDoctor, doctor_seleccionado->nombre);
	}break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
			case TXT_NOMBRE_CLIENTE:
			{
				

			}break;
			case LST_MASCOTAS:
			{
				if (HIWORD(wParam) == LBN_SELCHANGE)
				{
					idx = SendDlgItemMessage(hwnd, LST_MASCOTAS, LB_GETCURSEL, 0, 0);
					SendDlgItemMessage(hwnd, LST_MASCOTAS, LB_GETTEXT, idx, (LPARAM)hora);

					//nodo_doctores* doctor_sel = buscar_doctor(nombre);

					if (buscar_cita_hora(hora) == 1)
					{
						SetWindowText(hHora, aux_mascotas->hora);
						SetWindowText(hNombre, aux_mascotas->nombre);
						SetWindowText(hTelefono, aux_mascotas->telefono);
						SetWindowText(hEspecie, aux_mascotas->especie);
						SetWindowText(hMascota, aux_mascotas->mascota);
						SetWindowText(hMotivo, aux_mascotas->motivo);
						SetWindowText(hCosto, aux_mascotas->costo);
						SetWindowText(hCostoTotal, aux_mascotas->costoTotal); 
						SetWindowText(hEstatus, aux_mascotas->estatus);
						
					}
					es_nuevo = 0;
				}

			}break;

			case BTN_VER_CITAS:
			{
				GetWindowText(hFecha, (LPTSTR)fecha, 100);

				leer_mascotas_fecha(hwnd, fecha);
				es_nuevo = 1;

			}break;

			case BTN_DOCTORES_CITAS: 
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					HWND hDoctores = CreateDialog(hInst, MAKEINTRESOURCE(DLG_DOCTORES), NULL, fDoctores);
					ShowWindow(hDoctores, 1);
				}
			}break;

			case BTN_NUEVO_CITA: //nueva compra
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					SetWindowText(hNombre, "");
					SetWindowText(hTelefono, "");
					SetWindowText(hEspecie, "");
					SetWindowText(hMascota, "");
					SetWindowText(hMotivo, "");
					SetWindowText(hCosto, "");
					SetWindowText(hCostoTotal, " ");
					SetWindowText(hEstatus, "");
					es_nuevo = 1;
				}
			}break;

			case BTN_BORRAR_CITA:
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					borrar_mascota(aux_mascotas);
					leer_mascotas(hwnd);
				}
			}break;

			case BTN_GUARDAR_CITA:
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					GetWindowText(hFecha, (LPTSTR)fecha, 100);
					GetWindowText(hHora, (LPTSTR)hora, 100);
					GetWindowText(hNombre, nombre, 100);
					GetWindowText(hTelefono, telefono, 100);
					GetWindow(hEspecie, especie);
					GetWindowText(hMascota, mascota, 100); //Nombre del evento
					GetWindowText(hMotivo, motivo, 100);
					GetWindow(hCosto, costo);
					GetWindow(hCostoTotal, costoTotal);
					GetWindowText(hEstatus, estatus, 100);

					if (es_nuevo == 1)
					{
						nodo_mascotas* nuevo_mascota = new nodo_mascotas;

						strcpy_s(nuevo_mascota->fecha, fecha);
						strcpy_s(nuevo_mascota->hora, hora);
						strcpy_s(nuevo_mascota->nombre, nombre);
						strcpy_s(nuevo_mascota->telefono, telefono);
						scanf_s(nuevo_mascota->especie, L"%d", &especie);
						scanf_s(nuevo_mascota->especie, L"%d", &cantidadBoletos);
						printf(nuevo_mascota->especie, L"%d", cantidadBoletos);
						scanf_s(nuevo_mascota->costo, L"%d", &costo);
						cantidadBoletos = especie;
						strcpy_s(nuevo_mascota->mascota, mascota);
						//strcpy_s(nuevo_mascota->motivo, motivo);
						costo = 45 * cantidadBoletos;
						printf(nuevo_mascota->costo, L"%d", costo);
						scanf_s(nuevo_mascota->costoTotal, L"%f", &costoTotal);
						if (LOWORD(wParam) == IDC_DESCUENTO21 && HIWORD(wParam) == BN_CLICKED)
						{
							costo = costo / 2;
							costoTotal = costo + (costo * 0.16);
							printf(nuevo_mascota->costoTotal, L"%f", costoTotal);
						}
						else
						{
							if (LOWORD(wParam) == IDC_DESCUENTO10 && HIWORD(wParam) == BN_CLICKED)
							{
								costo = costo - (costo * 0.10);
								costoTotal = costo + (costo * 0.16);
								printf(nuevo_mascota->costoTotal, L"%f", costoTotal);
							}
							else
							{
								costoTotal = costo + (costo * 0.16);
								printf(nuevo_mascota->costoTotal, L"%f", costoTotal);
							}
						}
						strcpy_s(nuevo_mascota->estatus, estatus);
						strcpy_s(nuevo_mascota->doctor_guardo, doctor_seleccionado->nombre); //Perfil en uso

						agregar_mascota(nuevo_mascota);

						strcpy_s(mensaje, "Compra: ");
						strcat_s(mensaje, nuevo_mascota->nombre);
						strcat_s(mensaje, ", Se realizo con exito");

					}
					else
					{
						strcpy_s(aux_mascotas->fecha, fecha);
						strcpy_s(aux_mascotas->hora, hora);
						strcpy_s(aux_mascotas->nombre, nombre);
						strcpy_s(aux_mascotas->telefono, telefono);
						scanf_s(aux_mascotas->especie, especie);
						printf(aux_mascotas->especie, especie); //Cantidad de tickets
						cantidadBoletos = especie;
						strcpy_s(aux_mascotas->mascota, mascota);
						strcpy_s(aux_mascotas->motivo, motivo);
						costo = 45 * cantidadBoletos;
						printf(aux_mascotas->costo, L"%d", costo);
						strcpy_s(aux_mascotas->estatus, estatus);
						strcpy_s(aux_mascotas->doctor_guardo, doctor_seleccionado->nombre);

						strcpy_s(mensaje, "Compra: ");
						strcat_s(mensaje, aux_mascotas->nombre);
						strcat_s(mensaje, ", ha sido modificada");
					}

					MessageBox(0, mensaje, "Compras",
						MB_ICONINFORMATION + MB_OK);

					leer_mascotas(hwnd);
				}
			}break;
			case BTN_SALIR_CITAS:
			{
				if (HIWORD(wParam) == BN_CLICKED)
				{
					DestroyWindow(hwnd);
				}
			}break;
		}
	}break;
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		break;
	}
	case WM_DESTROY:
		DestroyWindow(hwnd);
		break;
	}
	return FALSE;
}

//Ventana de Perfiles
INT_PTR CALLBACK fDoctores(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int idx = 0;
	char nombre[100];
	char usuario[20];
	char pass[20];
	char cedula[20]; //Se utiliza como la edad
	char foto[MAX_PATH];

	HWND hNombre = GetDlgItem(hwnd, TXT_NOMBRE);
	HWND hUsuario = GetDlgItem(hwnd, TXT_USUARIO);
	HWND hPass = GetDlgItem(hwnd, TXT_PASSWORD);
	HWND hCedula = GetDlgItem(hwnd, TXT_CEDULA); //Edad
	HWND hRutaFoto = GetDlgItem(hwnd, TXT_RUTAFOTO);

	switch (msg)
	{
	case WM_INITDIALOG:
	{
		leer_doctores(hwnd);
		//cargar_imagen(hwnd, nombrearchivo, PCT_FOTO);
		/*foto = (HBITMAP)LoadImage(NULL, "foto.bmp", IMAGE_BITMAP, 120, 150, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, PCT_FOTO, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)foto);
		*/

	}break;
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case LST_DOCTORES:
		{
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				idx = SendDlgItemMessage(hwnd, LST_DOCTORES, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, LST_DOCTORES, LB_GETTEXT, idx, (LPARAM)nombre);

				//nodo_doctores* doctor_sel = buscar_doctor(nombre);

				if (buscar_doctor(nombre) == 1)
				{
					SetWindowText(hNombre, aux->nombre);
					SetWindowText(hUsuario, aux->usuario);
					SetWindowText(hPass, aux->pass);
					SetWindowText(hCedula, aux->cedula); //edad
					SetWindowText(hRutaFoto, aux->foto);

					cargar_imagen(hwnd, aux->foto, PCT_FOTO);
				}

				es_nuevo = 0;

			}

		}break;

		case BTN_BORRAR:
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				borrar_doctor(aux);
				leer_doctores(hwnd);
			}
		}break;

		case BTN_NUEVO:
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				SetWindowText(hNombre, "");
				SetWindowText(hUsuario, "");
				SetWindowText(hPass, "");
				SetWindowText(hCedula, ""); //Edad
				SetWindowText(hRutaFoto, "");
				es_nuevo = 1;
			}
		}break;

		case BTN_GUARDAR:
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				GetWindowText(hNombre, nombre, 100);
				GetWindowText(hUsuario, usuario, 20);
				GetWindowText(hPass, pass, 20);
				GetWindowText(hCedula, cedula, 20); //edad
				GetWindowText(hRutaFoto, foto, MAX_PATH);


				if (es_nuevo == 1)
				{
					nodo_doctores* nuevo = new nodo_doctores;

					strcpy_s(nuevo->nombre, nombre);
					strcpy_s(nuevo->usuario, usuario);
					strcpy_s(nuevo->pass, pass);
					strcpy_s(nuevo->cedula, cedula); //edad
					strcpy_s(nuevo->foto, foto);

					agregar_doctor(nuevo);

					strcpy_s(mensaje, "Perfil: ");
					strcat_s(mensaje, nuevo->nombre);
					strcat_s(mensaje, ", ha sido agregado");

				}
				else
				{
					strcpy_s(aux->nombre, nombre);
					strcpy_s(aux->usuario, usuario);
					strcpy_s(aux->pass, pass);
					strcpy_s(aux->cedula, cedula); //edad
					strcpy_s(aux->foto, foto);

					strcpy_s(mensaje, "Perfil: ");
					strcat_s(mensaje, aux->nombre);
					strcat_s(mensaje, ", ha sido modificado");
				}

				// prepara el mensaje
				MessageBox(0, mensaje, "Agregar Perfiles",
					MB_ICONINFORMATION + MB_OK);

				leer_doctores(hwnd);
			}
		}break;

		case BTN_CARGARFOTO:
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				examinar(hwnd);

				if (strcmp(nombrearchivo, "") != 0)
				{
					SendDlgItemMessage(hwnd, TXT_RUTAFOTO, WM_SETTEXT, 0, (LPARAM)nombrearchivo);

					cargar_imagen(hwnd, nombrearchivo, PCT_FOTO);
					//strcpy_s(aux->foto, nombrearchivo);

				}
			}
		}break;
		}
	}break;
	case WM_CLOSE:
	{
		DestroyWindow(hwnd);
		
		break;
	}
	case WM_DESTROY:
		//PostQuitMessage(117);
		break;

	}
	return FALSE;
}

nodo_doctores* buscar_doctor_ant(char* valor)
{
	nodo_doctores* doctor = 0;
	aux = 0;
	aux = lista_doctores;
	while (aux != 0)
	{
		if (strcmp(aux->nombre,valor) == 1) {
			doctor = aux;
			return doctor;
			break;
		}
		aux = aux->sig;
	}
	return 0;
}

int buscar_doctor(char* valor)
{
	aux = lista_doctores;
	while (aux != 0)
	{
		if (strcmp(aux->nombre, valor) == 0) {
			return 1;

		}
		aux = aux->sig;
	}
	return 0;
}

int buscar_cita_nombre(char* valor)
{
	aux_mascotas = lista_mascotas;
	while (aux_mascotas != 0)
	{
		if (strcmp(aux_mascotas->nombre, valor) == 0) {
			return 1;

		}
		aux_mascotas = aux_mascotas->sig;
	}
	return 0;
}

int buscar_cita_hora(char* valor)
{
	aux_mascotas = lista_mascotas;
	while (aux_mascotas != 0)
	{
		if (strcmp(aux_mascotas->hora, valor) == 0) {
			return 1;

		}
		aux_mascotas = aux_mascotas->sig;
	}
	return 0;
}

//Inicio de sesion
int login_doctor(char* valor)
{
	aux = lista_doctores;
	while (aux != 0)
	{
		if (strcmp(aux->usuario, valor) == 0) {
			return 1;

		}
		aux = aux->sig;
	}
	return 0;
}

//Leer perfiles
void leer_doctores(HWND hwnd)
{
	int idx=0;
	SendDlgItemMessage(hwnd, LST_DOCTORES, LB_RESETCONTENT, 0, 0);

	aux = lista_doctores;
	while (aux != 0)
	{
		// por cada nodo, agregar al LISTBOX             ​
		SendDlgItemMessage(hwnd, LST_DOCTORES, LB_ADDSTRING,
			0,
			(LPARAM)aux->nombre);
		idx++;
		aux = aux->sig;
	}

}

//Se utiliza para buscar la fecha del evento para comprar los tickets
void leer_mascotas_fecha(HWND hwnd, char* valor)
{
	int idx = 0;
	SendDlgItemMessage(hwnd, LST_MASCOTAS, LB_RESETCONTENT, 0, 0);

	aux_mascotas = lista_mascotas;
	while (aux_mascotas != 0)
	{
		if (strcmp(aux_mascotas->fecha, valor) == 0) {
		// por cada nodo, agregar al LISTBOX             ​
			SendDlgItemMessage(hwnd, LST_MASCOTAS, LB_ADDSTRING,
			0,
			(LPARAM)aux_mascotas->hora);
		}
		idx++;
		aux_mascotas = aux_mascotas->sig;
	}

}

//Se convierte a nombre del evento
void leer_mascotas(HWND hwnd)
{
	int idx = 0;
	SendDlgItemMessage(hwnd, LST_MASCOTAS, LB_RESETCONTENT, 0, 0);

	aux_mascotas = lista_mascotas;
	while (aux_mascotas != 0)
	{
		// por cada nodo, agregar al LISTBOX             ​
		SendDlgItemMessage(hwnd, LST_MASCOTAS, LB_ADDSTRING,
			0,
			(LPARAM)aux_mascotas->hora);
		idx++;
		aux_mascotas = aux_mascotas->sig;
	}

}

//Sin cambios
void agregar_estatus(HWND hwnd)
{
	SendDlgItemMessage(hwnd, CMB_ESTATUS, CB_RESETCONTENT, 0, 0);
	SendDlgItemMessage(hwnd, CMB_ESTATUS, CB_ADDSTRING, NULL, (LPARAM)"Pendiente");
	SendDlgItemMessage(hwnd, CMB_ESTATUS, CB_ADDSTRING, NULL, (LPARAM)"Cancelada");
	SendDlgItemMessage(hwnd, CMB_ESTATUS, CB_ADDSTRING, NULL, (LPARAM)"Efectuada");
}

//Se convirtio en el agregar perfil de usuario
void agregar_doctor(nodo_doctores* aux) {
	aux->ant = 0;
	aux->sig = 0;
	if (lista_doctores == 0) { // está vacía
		lista_doctores = aux;
	}
	else { // no está vacía
		ultimo_doctor->sig = aux;
		aux->ant = ultimo_doctor;
	}
	ultimo_doctor = aux;
}

//Agregar evento
void agregar_mascota(nodo_mascotas* aux_mascotas) {
	aux_mascotas->ant = 0;
	aux_mascotas->sig = 0;
	if (lista_mascotas == 0) { // está vacía
		lista_mascotas = aux_mascotas;
	}
	else { // no está vacía
		ultimo_mascota->sig = aux_mascotas;
		aux_mascotas->ant = ultimo_mascota;
	}
	ultimo_mascota= aux_mascotas;
}

//Borrar perfil
void borrar_doctor(nodo_doctores* aux) {
	// si el nodo a borrar es el primero
	if (aux == lista_doctores) {
		lista_doctores = aux->sig;
		lista_doctores->ant = 0;
	}
	else {
		// si el nodo a borrar es el ultimo
		if (aux == ultimo_doctor)
		{
			ultimo_doctor = aux->ant;
			ultimo_doctor->sig = 0;
		}
		else
		{
			// modificar ant y sig de los nodos vecinos
			nodo_doctores* prev = aux->ant;
			nodo_doctores* next = aux->sig;
			prev->sig = aux->sig;
			if (next != 0) {
				next->ant = aux->ant;
			}
		}

	}
	delete aux;
}

//Borrar evento
void borrar_mascota(nodo_mascotas* aux_mascota) {
	// si el nodo a borrar es el primero
	if (aux_mascota == lista_mascotas) {
		lista_mascotas = aux_mascota->sig;
		lista_mascotas->ant = 0;
	}
	else {
		// si el nodo a borrar es el ultimo
		if (aux_mascota == ultimo_mascota)
		{
			ultimo_mascota = aux_mascota->ant;
			ultimo_mascota->sig = 0;
		}
		else
		{
			// modificar ant y sig de los nodos vecinos
				nodo_mascotas* prev = aux_mascota->ant;
			nodo_mascotas* next = aux_mascota->sig;
			prev->sig = aux_mascota->sig;
			if (next != 0) {
				next->ant = aux_mascota->ant;
			}
		}

	}
	delete aux_mascota;
}

void leer_archivo_doctores(char* archivo) {
	ifstream arch_doctores;
	arch_doctores.open(archivo, ios::binary);
	if (arch_doctores.is_open())
	{
		// leer el archivo, hasta el fin 
		nuevo_doctor = new nodo_doctores;
		arch_doctores.read((char*)nuevo_doctor, sizeof(nodo_doctores));
		while (!arch_doctores.eof())
		{
			aux = new nodo_doctores;
			aux = nuevo_doctor;

			agregar_doctor(aux);

			nuevo_doctor = new nodo_doctores;
			arch_doctores.read((char*)nuevo_doctor, sizeof(nodo_doctores));
		}
		arch_doctores.close();
		//printf("archivo leído, lista ligada de alumnos creada\n");
		//system("pause");
	}
}

void leer_archivo_mascotas(char* archivo) {
	ifstream arch_mascotas;
	arch_mascotas.open(archivo, ios::binary);
	if (arch_mascotas.is_open())
	{
		// leer el archivo, hasta el fin 
		nuevo_mascota = new nodo_mascotas;
		arch_mascotas.read((char*)nuevo_mascota, sizeof(nodo_mascotas));
		while (!arch_mascotas.eof())
		{
			aux_mascotas = new nodo_mascotas;
			aux_mascotas = nuevo_mascota;

			agregar_mascota(aux_mascotas);

			nuevo_mascota = new nodo_mascotas;
			arch_mascotas.read((char*)nuevo_mascota, sizeof(nodo_mascotas));
		}
		arch_mascotas.close();
		//printf("archivo leído, lista ligada de alumnos creada\n");
		//system("pause");
	}
}


void escribir_archivo_doctores(char* archivo) {
	ofstream archi;
	archi.open(archivo, ios::trunc | ios::binary);
	if (archi.is_open())
	{
		// recorrer toda la lista ligada, nodo x nodo
		aux = lista_doctores;
		while (aux != 0)
		{
			//guardar en el archivo cada nodo
			archi.write((char*)aux, sizeof(nodo_doctores));

			//leer el siguiente nodo
			aux = aux->sig;
		}
		archi.close();
		//printf("Archivo guardado\n\n");
	}
}

void escribir_archivo_mascotas(char* archivo) {
	ofstream archi;
	archi.open(archivo, ios::trunc | ios::binary);
	if (archi.is_open())
	{
		// recorrer toda la lista ligada, nodo x nodo
		aux_mascotas = lista_mascotas;
		while (aux_mascotas != 0)
		{
			//guardar en el archivo cada nodo
			archi.write((char*)aux_mascotas, sizeof(nodo_mascotas));

			//leer el siguiente nodo
			aux_mascotas = aux_mascotas->sig;
		}
		archi.close();
		//printf("Archivo guardado\n\n");
	}
}

void cargar_imagen(HWND hDlg, char file[], int Pic) {
	static HBITMAP bmp1, bmp2;

	//Al objeto bmp1, se le asigna sin imagen:
	bmp1 = (HBITMAP)SendDlgItemMessage(hDlg, Pic, STM_GETIMAGE, IMAGE_BITMAP, 0);

	//Al objeto bmp2, se le asigna una imagen local:
	bmp2 = (HBITMAP)LoadImage(NULL, file, IMAGE_BITMAP, 120, 150, LR_LOADFROMFILE);

	SendDlgItemMessage(
		hDlg,
		Pic,
		STM_SETIMAGE,
		IMAGE_BITMAP,
		(LPARAM)bmp2);
}

void examinar(HWND hDlg) {
	char szFile[MAX_PATH] = { 0 };
	char tipos[200] = { 0 };
	// buffer for file name​
	OPENFILENAME ofn;       // common dialog box structure​
	HANDLE hf = nullptr;               // file handle​

	strcpy_s(tipos, "Todos\0*.*\0Imagen BMP\0*.bmp");

	// Initialize OPENFILENAME​

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hDlg;
	ofn.lpstrFile = szFile;

	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrFilter = "Todos\0*.*\0Mapa de Bits\0*.bmp";

	ofn.nFilterIndex = 2;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. ​
	if (GetOpenFileName(&ofn) == TRUE) {
		//
		strcpy_s(nombrearchivo, ofn.lpstrFile);
	}
	else {
		strcpy_s(nombrearchivo, "");
		MessageBox(hDlg, "No eligió archivo", "Aviso", MB_OK | MB_ICONINFORMATION);
	}
	//return szFile;
}