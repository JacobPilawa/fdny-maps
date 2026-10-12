import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Executes 00481BA0 with its intrusive-list registration service intercepted. */
public class BModelCandidateIteratorPcodeOracle extends GhidraScript {
 private static final long ENTRY=0x00481ba0L, REGISTER=0x00481b00L;
 private static final long GLOBAL=0x00502aacL, MANAGER=0x20010000L, LIST=MANAGER+0x54;
 private static final long NODES=0x20020000L, OBJECTS=0x20030000L, STACK=0x20fff000L, RETURN=0x70000000L;
 private EmulatorHelper emu;
 private Address a(long n){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(n);}
 private void w(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private int r(long p)throws Exception{byte[]b=emu.readMemory(a(p),4);return (b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private void ret(int args,long eax)throws Exception{long sp=emu.readRegister("ESP").longValue();emu.writeRegister("EAX",eax);emu.writeRegister("ESP",sp+4+args);emu.getEmulator().setExecuteAddress(Integer.toUnsignedLong(r(sp)));}
 private String longs(List<Long>x){List<String>s=new ArrayList<>();for(long v:x)s.add(Long.toUnsignedString(v));return "["+String.join(",",s)+"]";}
 private String sample(String name,int count)throws Exception{
  emu=new EmulatorHelper(currentProgram);emu.writeMemory(a(MANAGER),new byte[0x100]);emu.writeMemory(a(NODES),new byte[0x100]);emu.writeMemory(a(OBJECTS),new byte[0x100]);emu.writeMemory(a(STACK-0x100),new byte[0x300]);
  for(String reg:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(reg,0);emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
  w(GLOBAL,(int)MANAGER);w(MANAGER+0x50,0); // 00481BA0 adds 4, yielding manager+0x54
  long[]nodes=new long[count];List<Long>objects=new ArrayList<>(),controlledOrder=new ArrayList<>();
  for(int i=0;i<count;i++){nodes[i]=NODES+0x10L+i*8;long object=OBJECTS+0x20L+i*0x20;objects.add(object);w(nodes[i],(int)object);w(nodes[i]+4,i+1<count?(int)(nodes[i+1]):0);}
  w(LIST+0x10,count==0?0:(int)nodes[0]);
  w(STACK,(int)RETURN);emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",(int)(STACK-0x80));emu.getEmulator().setExecuteAddress(ENTRY);
  for(int steps=0;steps<400;steps++){long pc=emu.getExecutionAddress().getOffset();if(pc==RETURN)break;if(pc==REGISTER){long recv=emu.readRegister("ECX").longValue();if(recv!=LIST)throw new IllegalStateException("00481B00 receiver is not list group");ret(4,recv);continue;}if(!emu.step(monitor))throw new IllegalStateException("P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());}
  if(emu.getExecutionAddress().getOffset()!=RETURN)throw new IllegalStateException("00481BA0 instruction limit");
  long cursor=STACK-0x80;long list=Integer.toUnsignedLong(r(cursor)),node=Integer.toUnsignedLong(r(cursor+4));
  // The current cursor is the original routine's output; source-authored
  // node links/objects below are the controlled list input used by the caller.
  controlledOrder.addAll(objects);
  return "{\"name\":\""+name+"\",\"controlled_input_objects\":"+longs(objects)+",\"list_address\":"+list+",\"first_node\":"+node+",\"controlled_expected_order\":"+longs(controlledOrder)+"}";
 }
 @Override public void run()throws Exception{String[]args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: BModelCandidateIteratorPcodeOracle.java output.json original.exe");StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");String[]rows={sample("empty",0),sample("single",1),sample("ordered_bmodel_candidates",4)};Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"00481BA0\",\"scope\":\"Original P-code comparison covers only 00481BA0 cursor list-address and first-node initialization. 00481B00 list-registration is intercepted; old-cursor destruction is not exercised. Expected traversal order is controlled/source-derived and is not emitted by the original oracle.\",\"samples\":["+String.join(",",rows)+"]}\n");println("Captured original BModel candidate cursor initialization cases");}
}
