Compilation and Execution Instructions

1. Compile server.c, request.c, and siteCheck.c on the cse01.cse.unt.edu machine -> gcc server.c request.c siteCheck.c -o server
2. Compile client.c on another cse machine cse<02-06>.cse.unt.edu -> gcc client.c -o client
3. Start the server passing a port number as an argument -> ./server 5000
4. Start the client passing the same port number as an argument -> ./client 5000
5. On the client computer, request a web site in the form www.<sitename>.com
6. Type 'bye' to exit the program
