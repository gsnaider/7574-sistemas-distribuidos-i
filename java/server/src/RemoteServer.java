import com.Server;

import java.rmi.RemoteException;
import java.rmi.server.UnicastRemoteObject;

public class RemoteServer extends UnicastRemoteObject implements Server {

    private static final long serialVersionUID = 1L;

    protected RemoteServer() throws RemoteException {
        super();
    }

    @Override
    public String removerVocales(String s) throws RemoteException {
        System.out.println("Me llamaron con " + s);
        return s.replaceAll("[aeiouAEIOU]","");
    }

}
