/* Author:      Matt W. Martin, 4374851
 *              kaethis@tasmantis.net
 *
 * Project:     CS3790, Lab 03:
 *		Producer-Consumer Solution
 *              Java Implementation
 *
 * File:        ProdCons.java */

import java.lang.Math;
import java.util.concurrent.Semaphore;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class ProdCons{

	static final int  BUFFER_SIZE = 10;

	static int  items = 0;
    

	static final Semaphore  avail = new Semaphore(0, true);

	static final Semaphore  empty = new Semaphore(BUFFER_SIZE, true);

	static final Lock  mut = new ReentrantLock(true);


	static final String  enter_msg = " is entering CRITICAL REGION!\n";

	static final String  exit_msg = " is exiting CRITICAL REGION!\n";

	static final String  prod_msg = " is producing an item...\n";

	static final String  cons_msg = " is consuming an item...\n";

	static final String  item_msg = "\n Items in buffer is now %d.\n";


	static void work(){
	/* This METHOD simulates some sort of undetermined number of unknown
	 *  instructions executed by the THREAD.  In reality, all it does is put the
	 *  THREAD to sleep for an indescriminate amount of time (between 1 and 5
	 *  seconds). */

		int  time = (int)Math.ceil(Math.random()*5);

		time *= 1000;	// NOTE: The PARAM for Thread.sleep() is in millisec.

		try{
			Thread.sleep(time);

		} catch(InterruptedException ie){ }
	}

	static class ProducerThread extends Thread{

		int  tid;

		static void produce(){

			work();

			items++;
		}

		ProducerThread(int thread_id){

			tid = thread_id;
		}

		public void run(){   

			while(true){

				work();

				try{
					empty.acquire();

				} catch(InterruptedException ie){ }

				  System.out.printf("\nPROD#%d" + enter_msg, tid);

				  mut.lock();

				    System.out.printf("\n PROD#%d" + prod_msg, tid);

				    produce();

				    System.out.printf(item_msg, items);

				  mut.unlock();

				  System.out.printf("\nPROD#%d" + exit_msg, tid);

				avail.release();


				work();
			}
		}
	}


	static class ConsumerThread extends Thread{

		int  tid;

		static void consume(){

			work();

			items--;
		}

		ConsumerThread(int thread_id){

			tid = thread_id;

		}

		public void run(){

			while(true){

				work();


				try{
					avail.acquire();

				} catch(InterruptedException ie){ }

				  System.out.printf("\nCONS#%d" + enter_msg, tid);

				  mut.lock();

				    System.out.printf("\n CONS#%d" + cons_msg, tid);

				    consume();

				    System.out.printf(item_msg, items);

				  mut.unlock();

				  System.out.printf("\nCONS#%d" + exit_msg, tid);

				empty.release();


				work();
			}
		}
	}


	public static void main(String[] args) {

		String  exec_syntax = "java ProdCons <prod_num> <cons_num>\n\n";

		if(args.length != 2){
            
			System.out.print("\nINVALID NUMBER OF ARGS!\n");
			System.out.print("NOTE:  " + exec_syntax);
			System.exit(-1);
		}
        

		int  prod_num = Integer.parseInt(args[0]);

		int  cons_num = Integer.parseInt(args[1]);
        

		for(int i = 0; i < prod_num; i++)
			(new ProducerThread(i)).start();

		for(int i = 0; i < cons_num; i++)
			(new ConsumerThread(i)).start();

	}
}
