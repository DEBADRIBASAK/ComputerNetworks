default:
	gcc -c MathClient.c -o MathClient.o
	gcc -c MathSpec_clnt.c -o MathSpec_clnt.o
	gcc -c MathSpec_xdr.c -o MathSpec_xdr.o
	gcc -o MathClient MathClient.o MathSpec_clnt.o MathSpec_xdr.o
	gcc -c MathServer.c -o MathServer.o
	gcc -c MathSpec_svc.c -o MathSpec_svc.o
	gcc -o MathServer MathServer.o MathSpec_svc.o MathSpec_xdr.o