
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.Statement;
import java.util.ArrayList;

/**
 * �����߳�
 * 
 *
 */
public class M2MSend extends Thread {
	
	//�û�����
	private ArrayList<Socket> list = M2MServer.getList();
	//��ǰ�û�
	private Socket s;
	
	public M2MSend(Socket s){
		this.s = s;
	}
	
	public void run(){
		
		//��ȡ���û� IP
		String ip = s.getInetAddress().getHostAddress();
		
		try {
			
			//��ȡ�û���Ϣ
			BufferedReader reader = new BufferedReader(new InputStreamReader(s.getInputStream()));
			
			//���ϵĶ�ȡд������
			while(true){
				
				//��������
				String info = null;
				info=reader.readLine();
			
				//�����ȡ��Ϣ��Ϊ��
				//if((info=reader.readLine()) != null){
				String sql="select comd from order where name ="+info+";";
				String sql2="delete from order where name ="+info+";";
	    		 try{
	    			 Statement stmt = null;
	    			 Connection conn = null;
	    			 conn=DB.getConn();
	    			 stmt=DB.createStmt(conn);
	    			 ResultSet rs = stmt.executeQuery(sql);
	    			 while(rs.next()){
	    				 info=rs.getString("comd");
	    			 }
	    			 stmt.execute(sql2);
	    			 }
	    		 catch(Exception e){
	    			 e.printStackTrace();
	    			 }
					
						//��ȡ����������
						PrintWriter pw;
						pw = new PrintWriter(s.getOutputStream());
						//д����Ϣ
						pw.println(info);
						pw.flush();
						System.out.println(info);
						Thread.sleep(5000);
					
				//}
			}
			
		} catch (IOException | InterruptedException e1) {
			//�û�����
			list.remove(s);
			System.err.println(ip + " ������ , ��ǰ��������Ϊ: " + list.size() + " �� !");
		}
		
		
	}

}
