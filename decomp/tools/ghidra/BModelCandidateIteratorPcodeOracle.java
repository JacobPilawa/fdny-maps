import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Executes caller-facing 00481C00 with its intrusive-list registration service intercepted. */
public class BModelCandidateIteratorPcodeOracle extends GhidraScript {
 private static final long ENTRY=0x00481c00L, INIT_CURSOR=0x00481ba0L, REGISTER=0x00481b00L;
 private static final long GLOBAL=0x00502aacL, MANAGER=0x20010000L, LIST=MANAGER+0x54;
 private static final long NODES=0x20020000L, OBJECTS=0x20030000L, STACK=0x20fff000L, RETURN=0x70000000L;
 private EmulatorHelper emu;
 private Address a(long n){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(n);}
 private void w(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
 private int r(long p)throws Exception{byte[]b=emu.readMemory(a(p),4);return (b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
 private void ret(int args,long eax)throws Exception{long sp=emu.readRegister("ESP").longValue();emu.writeRegister("EAX",eax);emu.writeRegister("ESP",sp+4+args);emu.getEmulator().setExecuteAddress(Integer.toUnsignedLong(r(sp)));}
 private String longs(List<Long>x){List<String>s=new ArrayList<>();for(long v:x)s.add(Long.toUnsignedString(v));return "["+String.join(",",s)+"]";}
 private String sample(String name,int count,boolean nullSentinel)throws Exception{
  emu=new EmulatorHelper(currentProgram);emu.writeMemory(a(MANAGER),new byte[0x100]);emu.writeMemory(a(NODES),new byte[0x100]);emu.writeMemory(a(OBJECTS),new byte[0x100]);emu.writeMemory(a(STACK-0x100),new byte[0x300]);
  for(String reg:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})emu.writeRegister(reg,0);emu.writeRegister("DF",0);emu.writeRegister("EFLAGS",0x202);
  int managerWord=nullSentinel?-0x50:(int)MANAGER;w(GLOBAL,managerWord);w(MANAGER+0x50,0); // ordinary path resolves manager+0x54
  long[]nodes=new long[count];List<Long>objects=new ArrayList<>(),controlledOrder=new ArrayList<>();
  for(int i=0;i<count;i++){nodes[i]=NODES+0x10L+i*8;long object=OBJECTS+0x20L+i*0x20;objects.add(object);w(nodes[i],(int)object);w(nodes[i]+4,i+1<count?(int)(nodes[i+1]):0);}
  w(LIST+0x10,count==0?0:(int)nodes[0]);
  long cursor=STACK-0x80;w(cursor,0xdeadbeef);w(cursor+4,0xcafebabe);
  w(STACK,(int)RETURN);emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",(int)cursor);emu.getEmulator().setExecuteAddress(ENTRY);
  long listAtNestedCall=-1,nodeAtNestedCall=-1,registrationReceiver=-1;
  for(int steps=0;steps<600;steps++){long pc=emu.getExecutionAddress().getOffset();if(pc==RETURN)break;if(pc==INIT_CURSOR){listAtNestedCall=Integer.toUnsignedLong(r(cursor));nodeAtNestedCall=Integer.toUnsignedLong(r(cursor+4));}if(pc==REGISTER){registrationReceiver=Integer.toUnsignedLong(emu.readRegister("ECX").intValue());ret(4,registrationReceiver);continue;}if(!emu.step(monitor))throw new IllegalStateException("P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());}
  if(emu.getExecutionAddress().getOffset()!=RETURN)throw new IllegalStateException("00481C00 instruction limit at "+emu.getExecutionAddress());
  long list=Integer.toUnsignedLong(r(cursor)),node=Integer.toUnsignedLong(r(cursor+4));
  // The current cursor is the original routine's output; source-authored
  // node links/objects below are the controlled list input used by the caller.
  controlledOrder.addAll(objects);
  return "{\"name\":\""+name+"\",\"null_list_sentinel\":"+nullSentinel+",\"controlled_input_objects\":"+longs(objects)+",\"cursor_zeroed_by_00481C00\":"+(listAtNestedCall==0&&nodeAtNestedCall==0)+",\"cursor_words_at_00481BA0\":["+listAtNestedCall+","+nodeAtNestedCall+"],\"registration_receiver\":"+registrationReceiver+",\"list_address\":"+list+",\"first_node\":"+node+",\"controlled_expected_order\":"+longs(controlledOrder)+"}";
 }
 @Override public void run()throws Exception{String[]args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: BModelCandidateIteratorPcodeOracle.java output.json original.exe");StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");String[]rows={sample("empty",0,false),sample("single",1,false),sample("ordered_bmodel_candidates",4,false)};String sentinel=sample("null_list_sentinel",0,true);Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"00481C00\",\"scope\":\"Original caller-facing 00481C00 zeroes both cursor words then invokes 00481BA0. P-code comparison covers the wrapper's zeroing and 00481BA0 cursor list-address/first-node initialization. Only 00481B00 registration is intercepted; expected traversal order is controlled/source-derived, not original oracle output. The null-list sentinel sample is separate because the native checker tests controlled list traversal only.\",\"samples\":["+String.join(",",rows)+"],\"null_list_sentinel_sample\":"+sentinel+"}\n");println("Captured original BModel 00481C00 iterator cases");}
}
