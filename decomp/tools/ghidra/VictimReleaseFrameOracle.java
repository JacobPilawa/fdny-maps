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
import java.util.List;

/** Execute release or passout prefix through its position stores, before floor. */
public class VictimReleaseFrameOracle extends GhidraScript {
 private static final long VICTIM=0x20000000L,PLAYER=0x20001000L,FRAME=0x20002000L;
 private static final long TABLE=0x20003000L,GETFRAME=0x20004000L,STACK=0x20fff000L;
 private static final long ENTRY=0x0045c000L,STOP=0x0045c0eeL;
 private Address a(long n){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(n);}
 private void word(EmulatorHelper e,long p,int v){e.writeMemory(a(p),ByteBuffer.allocate(4).order(ByteOrder.LITTLE_ENDIAN).putInt(v).array());}
 private int word(EmulatorHelper e,long p){return ByteBuffer.wrap(e.readMemory(a(p),4)).order(ByteOrder.LITTLE_ENDIAN).getInt();}
 private String array(float[] f){StringBuilder s=new StringBuilder("[");for(int i=0;i<f.length;i++){if(i>0)s.append(',');s.append(Float.toString(f[i]));}return s.append(']').toString();}
 public void run() throws Exception {
  String[] args=getScriptArgs();if(args.length<2||args.length>3)throw new IllegalArgumentException("output.json original.exe [drop]");
  boolean drop=args.length==3&&args[2].equals("drop");
  long entry=drop?0x0045c1a0L:ENTRY,stop=drop?0x0045c295L:STOP;
  if(!currentProgram.getLanguageID().toString().startsWith("x86:LE:32:"))throw new IllegalStateException("Expected x86 PE32");
  if(!getInstructionAt(a(stop)).toString().contains("0049da00"))throw new IllegalStateException("Wrong floor boundary");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
  if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Program/input hash mismatch");
  float[][] cases={
   {1,0,0, 0,0,1, 1472,-230.9f,-4972},
   {0,0,-1, 1,0,0, 1820,-230.9f,-4960},
   {-1,0,0, 0,0,-1, 0,0,0},
   {0,0,1, -1,0,0, 5,10,15},
   {.6f,0,-.8f, .8f,0,.6f, 100,-230.9f,-100},
   {1,0,0, 0,.8f,.6f, -17,25,4096},
   {1,0,0, 0,1,0, 100,20,300},
   {0,1,0, 0,1,0, 3,4,5}
  };
  List<String> rows=new ArrayList<>();
  int[] counts=drop?new int[]{6,5,0,-1}:new int[]{6};
  for(int count:counts)for(int n=0;n<cases.length;n++){
   float[] f=cases[n];EmulatorHelper e=new EmulatorHelper(currentProgram);
   try{
    e.writeMemory(a(VICTIM),new byte[0x400]);e.writeMemory(a(PLAYER),new byte[0x100]);e.writeMemory(a(FRAME),new byte[0x100]);
    word(e,VICTIM+0x24,0x004dfc30);word(e,PLAYER,(int)TABLE);word(e,TABLE+0x18,(int)GETFRAME);
    word(e,FRAME,0x004dfc30);
    for(int i=0;i<3;i++){word(e,FRAME+4+4*i,Float.floatToRawIntBits(f[i]));word(e,FRAME+0x1c+4*i,Float.floatToRawIntBits(f[3+i]));word(e,FRAME+0x28+4*i,Float.floatToRawIntBits(f[6+i]));}
    word(e,VICTIM+0x58,Float.floatToRawIntBits(2));word(e,VICTIM+0x5c,Float.floatToRawIntBits(3));word(e,VICTIM+0x60,Float.floatToRawIntBits(4));
    int seed=1234+n;
    if(drop){
     word(e,VICTIM+0x180,count);word(e,STACK,0x70000000);word(e,STACK+4,seed);e.writeRegister("ESP",STACK);e.getEmulator().setExecuteAddress(0x00404b50L);
     int seedSteps=0;while(e.getExecutionAddress().getOffset()!=0x70000000L&&seedSteps++<20000)if(!e.step(monitor))throw new IllegalStateException("Seed failure: "+e.getLastError());
     if(e.getExecutionAddress().getOffset()!=0x70000000L)throw new IllegalStateException("Seed timeout");
    }
    word(e,STACK,0x70000000);word(e,STACK+4,(int)PLAYER);
    for(String r:new String[]{"EAX","EBX","EDX","ESI","EDI","EBP"})e.writeRegister(r,0);
    e.writeRegister("ESP",STACK);e.writeRegister("ECX",VICTIM);e.writeRegister("EFLAGS",0x202);e.writeRegister("DF",0);
    e.writeRegister("FPUControlWord",0x037f);e.writeRegister("FPUStatusWord",0);e.writeRegister("FPUTagWord",0xffff);
    for(String r:new String[]{"ST0","ST1","ST2","ST3","ST4","ST5","ST6","ST7"})e.writeRegister(r,BigInteger.ZERO);
    e.getEmulator().setExecuteAddress(entry);int steps=0,getters=0,motions=0,motion=-1,speed=0,draws=0;
    while(e.getExecutionAddress().getOffset()!=stop&&steps++<20000){
     if(drop&&e.getExecutionAddress().getOffset()==0x00404a70L)draws++;
     if(drop&&e.getExecutionAddress().getOffset()==0x00480ea0L){
      long sp=e.readRegister("ESP").longValue();motion=word(e,sp+4);speed=word(e,sp+8);if(word(e,sp+12)!=0)throw new IllegalStateException("Expected animation flag zero");
      long ret=Integer.toUnsignedLong(word(e,sp));e.writeRegister("ESP",sp+16);e.getEmulator().setExecuteAddress(ret);motions++;continue;
     }
     if(e.getExecutionAddress().getOffset()==GETFRAME){long sp=e.readRegister("ESP").longValue();long ret=Integer.toUnsignedLong(word(e,sp));e.writeRegister("EAX",FRAME);e.writeRegister("ESP",sp+4);e.getEmulator().setExecuteAddress(ret);getters++;continue;}
     if(!e.step(monitor))throw new IllegalStateException("P-code failure "+e.getExecutionAddress()+": "+e.getLastError());
    }
    if(e.getExecutionAddress().getOffset()!=stop||getters!=3||(drop&&motions!=1))throw new IllegalStateException("Incomplete prefix");
    StringBuilder bits=new StringBuilder("[");for(int i=0;i<15;i++){if(i>0)bits.append(',');bits.append(Integer.toUnsignedLong(word(e,VICTIM+0x28+4*i)));}bits.append(']');
    rows.add("{\"input\":"+array(f)+",\"expected_words\":"+bits+",\"seed\":"+seed+",\"motion_count\":"+Integer.toUnsignedLong(count)+",\"motion\":"+motion+",\"speed_bits\":"+Integer.toUnsignedLong(speed)+",\"draws\":"+draws+",\"steps\":"+steps+"}");
    println("sample="+n+" steps="+steps+" getters="+getters);
   }finally{e.dispose();}
  }
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"entry\":\""+String.format("%08x",entry)+"\",\"stop\":\""+String.format("%08x",stop)+"\",\"scope\":\"frame prefix; getter/animation adapters; original drop RNG; no floor/event/timers\",\"samples\":["+String.join(",\n",rows)+"]}\n");
 }
}
