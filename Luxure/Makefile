serveur:
	g++ -o serveur src/serveur.cpp src/LPTF_Net/* -Wall -Wextra -Werror

client:
	g++ -o client src/client.cpp src/LPTF_Net/* -Wall -Wextra -Werror

clean:
	rm -f serveur.exe & rm -f serveur
	rm -f client.exe & rm -f client

fclean:
	rm -f serveur.exe & rm -f serveur
	rm -f client.exe & rm -f client

all: serveur client