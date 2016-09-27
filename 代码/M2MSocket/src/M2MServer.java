
import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;

/**
 * ��Զ� ������
 * 
 *
 */
public class M2MServer {
	
	//�û�����
	private static ArrayList<Socket> list = new ArrayList<Socket>();
	
	public static void main(String[] args) {
		
		//�����û�
		Socket s = null;
		
		//�û� 
		String ip = null;
		
		try {
			
			//��������������
			ServerSocket ss = new ServerSocket(1234);
			
			//���� �û�����
			list = new ArrayList<Socket>();
			
			System.out.println("������׼������ ...");
			
			//ѭ������
			while(true){
				
				//�����û�
				s = ss.accept();
				
				//���ߵ��˶���ӵ� ������
				list.add(s);
				
				//��ȡ Socket IP
				ip = s.getInetAddress().getHostAddress();
				System.err.println( ip + " �û������� , ��ǰ�����û�Ϊ: " + list.size() + "�� !" );
				
				//���� ������Ϣ�߳�
				M2MSend send = new M2MSend(s);
				send.start();
			
			}
			
		} catch (IOException e) {
			//�û�����
			list.remove(s);
			System.err.println(ip + " ������ , ��ǰ��������Ϊ: " + list.size() + " �� !");
		}
		
	}

	public static ArrayList<Socket> getList() {
		return list;
	}

	public static void setList(ArrayList<Socket> list) {
		M2MServer.list = list;
	}
	
	

}