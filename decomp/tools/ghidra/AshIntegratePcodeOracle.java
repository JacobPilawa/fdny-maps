import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Executes original 0047F2A0 and its real vector helpers under x86 P-code. */
public class AshIntegratePcodeOracle extends GhidraScript {
 private static final long OBJECT=0x20000000L, STACK=0x20fff000L, RET=0x70000000L;
 private static final long ENTRY=0x0047f2a0L;
 private Address a(long p){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(p);}
 private void w(EmulatorHelper e,long p,int v)throws Exception{e.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private int r(EmulatorHelper e,long p)throws Exception{byte[] b=e.readMemory(a(p),4);return (b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private void cpu(EmulatorHelper e)throws Exception {
  for(String n:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})e.writeRegister(n,0);
  e.writeRegister("DF",0);e.writeRegister("EFLAGS",0x202);
  Register cw=currentProgram.getLanguage().getRegister("FPUControlWord");if(cw!=null)e.writeRegister(cw,0x037fL);
  Register sw=currentProgram.getLanguage().getRegister("FPUStatusWord");if(sw!=null)e.writeRegister(sw,0);
  Register tag=currentProgram.getLanguage().getRegister("FPUTagWord");if(tag!=null)e.writeRegister(tag,0xffffL);
  for(Register reg:currentProgram.getLanguage().getRegisters())if(reg.getName().toUpperCase().matches("ST[0-7]"))e.writeRegister(reg,java.math.BigInteger.ZERO);
 }
 private String sample(String name,int dt,int[] pos,int[] vel,int[] acc,int[] origin)throws Exception {
  EmulatorHelper e=new EmulatorHelper(currentProgram);
  try {
   e.writeMemory(a(OBJECT),new byte[0x200]);e.writeMemory(a(STACK),new byte[0x100]);
   for(int i=0;i<3;i++){w(e,OBJECT+0x7c+4*i,pos[i]);w(e,OBJECT+0x88+4*i,vel[i]);w(e,OBJECT+0x94+4*i,acc[i]);w(e,OBJECT+0x5c+4*i,origin[i]);}
   // 004DC50C is the original image's binary32 +0.0 clamp sentinel; leave
   // that hash-pinned data unchanged and override only the live timestep.
   w(e,0x00502b04L,dt);
   cpu(e);w(e,STACK,(int)RET);e.writeRegister("ESP",STACK);e.writeRegister("ECX",OBJECT);
   e.getEmulator().setExecuteAddress(ENTRY);
   int steps=0;
   while(e.getExecutionAddress().getOffset()!=RET&&steps++<4000)
    if(!e.step(monitor))throw new IllegalStateException("P-code error at "+e.getExecutionAddress()+": "+e.getLastError());
   if(e.getExecutionAddress().getOffset()!=RET)throw new IllegalStateException("instruction limit at "+e.getExecutionAddress());
   return "{\"name\":\""+name+"\",\"dt\":"+Integer.toUnsignedLong(dt)+",\"position_before\":"+arr(pos)+",\"velocity_before\":"+arr(vel)+",\"acceleration_before\":"+arr(acc)+",\"origin_before\":"+arr(origin)+",\"position_after\":"+out(e,OBJECT+0x7c)+",\"origin_after\":"+out(e,OBJECT+0x5c)+"}";
  } finally {e.dispose();}
 }
 private String arr(int[] v){return "["+Integer.toUnsignedLong(v[0])+","+Integer.toUnsignedLong(v[1])+","+Integer.toUnsignedLong(v[2])+"]";}
 private String out(EmulatorHelper e,long p)throws Exception{return "["+Integer.toUnsignedLong(r(e,p))+","+Integer.toUnsignedLong(r(e,p+4))+","+Integer.toUnsignedLong(r(e,p+8))+"]";}
 @Override public void run()throws Exception {
  String[] args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: AshIntegratePcodeOracle.java output.json original.exe");
  StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
  if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
  int[][][] in={
   {{0x3f800000,0xc0000000,0x40800000},{0x40000000,0x3f000000,0xc0400000},{0x3f000000,0x40000000,0xbf800000},{0x41200000,0xc1200000,0x3f800000}},
   {{0x3f000000,0x3f000000,0x3f000000},{0x3f800000,0x40000000,0x40400000},{0x3f800000,0x3f800000,0x3f800000},{0,0,0}},
   {{0xbf000000,0x3f000000,0xbf000000},{0xc0000000,0x3f800000,0x40000000},{0x40000000,0xc0000000,0x3f800000},{0x3f800000,0x40000000,0x40400000}},
   {{0x3f000001,0x3effffff,0x3f000000},{0x3f800001,0xbf7fffff,0x40000001},{0x3f000001,0xbf000001,0x3f800001},{0x80000000,0x00000000,0x3f800000}},
   {{0x3727c5ac,0,0},{0,0,0},{0x3f800000,0,0},{0,0,0}},
   {{0x3727c5ab,0,0},{0,0,0},{0x3f800000,0,0},{0,0,0}},
   {{0,0,0},{0,0,0},{0xbf800000,0,0},{0,0,0}},
   {{0x3727c5ac,0,0},{0,0,0},{0xbf800000,0,0},{0,0,0}}
  };
  int[] dts={0x3c23d70a,0x3f000000,0x3e800000,0x3f000000,0x3f000000,0x3f000000,0x3f000000,0x3f000000};
  List<String> rows=new ArrayList<>();for(int i=0;i<in.length;i++)rows.add(sample("integrate_"+i,dts[i],in[i][0],in[i][1],in[i][2],in[i][3]));
  Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"0047F2A0\",\"scope\":\"Original routine and actual 00446390/0042D050 helpers; explicit global timestep, finite controlled vectors; x87 reset control 0x037f\",\"samples\":["+String.join(",",rows)+"]}\n");println("Captured "+rows.size()+" original ash integrate P-code cases");
 }
}
