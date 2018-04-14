package com.client;

import com.Constants;
import com.Server;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

public class Client {

    public static void main(String[] args) throws  Exception{
        Registry registry = LocateRegistry.getRegistry(Constants.RMI_PORT);
        Server server = (Server) registry.lookup(Constants.RMI_ID);

        String s = "HOLA";
        System.out.println("[CLIENT] Llamando a server con " + s);
        System.out.println("[CLIENT] Server devolvio " + server.removerVocales(s));
    }

}
