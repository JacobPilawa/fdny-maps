import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.math.BigInteger;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;

/** Execute original deadline arithmetic and its actual shared RNG consumer. */
public class PlayerRescueDeadlineOracle extends GhidraScript {
 private static final long OBJECT=0x20000000L,PLAYER=0x20001000L,SINGLETON=0x20010000L,APP=0x20011000L,GAME=0x20012000L,STACK=0x20fff000L,RETURN=0x70000000L;
 private Address a(long p){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(p);}
 private void w(EmulatorHelper e,long p,int v){e.writeMemory(a(p),ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(v).array());}
 private int r(EmulatorHelper e,long p){return ByteBuffer.wrap(e.readMemory(a(p),4)).order(ByteOrder.LITTLE_ENDIAN).getInt();}
 private int run(EmulatorHelper e,long entry,int seed) throws Exception {
  w(e,STACK,(int)RETURN);w(e,STACK+4,seed);e.writeRegister("ESP",STACK);e.writeRegister("ECX",OBJECT);e.getEmulator().setExecuteAddress(entry);
  int steps=0,draws=0;
  while(e.getExecutionAddress().getOffset()!=RETURN&&steps++<20000){
   if(e.getExecutionAddress().getOffset()==0x00404a70L)draws++;
   if(!e.step(monitor))throw new IllegalStateException("P-code error at "+e.getExecutionAddress()+": "+e.getLastError());
  }
  if(e.getExecutionAddress().getOffset()!=RETURN)throw new IllegalStateException("Instruction limit reached");
  return draws;
 }
 public void run() throws Exception {
  String[] args=getScriptArgs();if(args.length<2||args.length>3)throw new IllegalArgumentException("output.json original.exe [input]");
  boolean input=args.length==3&&args[2].equals("input");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
  if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Hash mismatch");
  ArrayList<String> rows=new ArrayList<>();
  float[] fields=input?new float[]{Float.NaN,0,180,600,7200}:new float[]{0,180,7200,7201};
  int[] clocks=input?new int[]{0}:new int[]{0,107999,108000,-16};
  for(float field:fields)for(int mode:new int[]{0,1,2})for(int now:clocks){
   EmulatorHelper e=new EmulatorHelper(currentProgram);
   try{
    e.writeMemory(a(OBJECT),new byte[0x100]);e.writeMemory(a(PLAYER),new byte[0x4000]);e.writeMemory(a(SINGLETON),new byte[0x100]);e.writeMemory(a(APP),new byte[0x800]);e.writeMemory(a(GAME),new byte[0x100]);
    w(e,0x00503a78L,(int)SINGLETON);w(e,SINGLETON+0x30,(int)PLAYER);w(e,PLAYER+0x343c,Float.floatToRawIntBits(field));w(e,0x00502aacL,(int)APP);w(e,APP+0x6e4,(int)GAME);w(e,GAME+0x2c,mode);w(e,0x00502afcL,now);
    for(String reg:new String[]{"EAX","EBX","EDX","ESI","EDI","EBP"})e.writeRegister(reg,0);
    e.writeRegister("DF",0);e.writeRegister("EFLAGS",0x202);e.writeRegister("FPUControlWord",0x037f);e.writeRegister("FPUStatusWord",0);e.writeRegister("FPUTagWord",0xffff);for(String reg:new String[]{"ST0","ST1","ST2","ST3","ST4","ST5","ST6","ST7"})e.writeRegister(reg,BigInteger.ZERO);
    int seed=1234,draws=0;
    if(input){
     long level=0x20013000L;w(e,APP+0x688,Float.isNaN(field)?0:(int)level);if(!Float.isNaN(field))w(e,level+0x10,Float.floatToRawIntBits(field));
     e.writeRegister("ESP",STACK);e.writeRegister("ESI",PLAYER);e.writeRegister("EBX",0);e.getEmulator().setExecuteAddress(0x0044de07L);
     int steps=0;while(e.getExecutionAddress().getOffset()!=0x0044de96L&&steps++<200)if(!e.step(monitor))throw new IllegalStateException("Input prefix error: "+e.getLastError());
     if(e.getExecutionAddress().getOffset()!=0x0044de96L)throw new IllegalStateException("Input prefix limit");
     rows.add("{\"level_value\":"+(Float.isNaN(field)?"null":Float.toString(field))+",\"mode\":"+mode+",\"bits\":"+Integer.toUnsignedLong(r(e,PLAYER+0x343c))+"}");
    }else{
     run(e,0x00404b50L,seed);draws=run(e,0x004541a0L,0);
     rows.add("{\"field\":"+field+",\"mode\":"+mode+",\"now\":"+Integer.toUnsignedLong(now)+",\"seed\":"+seed+",\"deadline\":"+Integer.toUnsignedLong(r(e,OBJECT+0x14))+",\"draws\":"+draws+"}");
    }
   }finally{e.dispose();}
  }
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"entry\":\""+(input?"0044de07":"004541a0")+"\",\"method\":\"original P-code with supplied globals; input mode stops at 0044de96, deadline mode executes original RNG\",\"samples\":["+String.join(",\n",rows)+"]}\n");println("Captured "+rows.size()+" original deadline calls");
 }
}
