package com;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface Server extends Remote {

    public String removerVocales(String s) throws RemoteException;

}
