all: serveur client

serveur:
	g++ -o serveur serveur.cpp commands.cpp LPTF_Net/* -Wall -Wextra -Werror

client:
	g++ -o client client.cpp commands.cpp LPTF_Net/* -Wall -Wextra -Werror

clean:
	rm -f serveur.exe & rm -f serveur
	rm -f client.exe & rm -f client

fclean:
	rm -f serveur.exe & rm -f serveur
	rm -f client.exe & rm -f client