
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
 * 发送线程
 * 
 *
 */
public class M2MSend extends Thread {
	
	//用户集合
	private ArrayList<Socket> list = M2MServer.getList();
	//当前用户
	private Socket s;
	
	public M2MSend(Socket s){
		this.s = s;
	}
	
	public void run(){
		
		//获取该用户 IP
		String ip = s.getInetAddress().getHostAddress();
		
		try {
			
			//读取用户信息
			BufferedReader reader = new BufferedReader(new InputStreamReader(s.getInputStream()));
			
			//不断的读取写出数据
			while(true){
				
				//接收数据
				String info = null;
				info=reader.readLine();
			
				//如果读取信息不为空
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
					
						//获取对象的输出流
						PrintWriter pw;
						pw = new PrintWriter(s.getOutputStream());
						//写入信息
						pw.println(info);
						pw.flush();
						System.out.println(info);
						Thread.sleep(5000);
					
				//}
			}
			
		} catch (IOException | InterruptedException e1) {
			//用户下线
			list.remove(s);
			System.err.println(ip + " 已下线 , 当前在线人数为: " + list.size() + " 人 !");
		}
		
		
	}

}
