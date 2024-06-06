# Para que esto ande hace chmod +x compiler.sh para agregarle execute (esto solo una vez)
# Descomenta ./run.sh si queres que corra tambien 
# Si cambias el nombre del container (o no sos martu) cambia el 

CONTAINER="amogos2"

docker exec -ti $CONTAINER make clean -C/root/Toolchain
docker exec -it $CONTAINER make clean -C/root/
docker exec -it $CONTAINER make -C/root/Toolchain
docker exec -ti $CONTAINER make -C/root/


# ./run.sh
