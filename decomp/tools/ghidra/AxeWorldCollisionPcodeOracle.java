import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import java.math.BigInteger;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Execute original 00492CE0; intercept only imported geWorld_Collision. */
public class AxeWorldCollisionPcodeOracle extends GhidraScript {
    private static final long ENTRY=0x00492ce0L, CALL=0x00492da0L;
    private static final long MODEL=0x20001000L, INPUT=0x20002000L, STACK=0x20fff000L, RETURN=0x70000000L;
    private EmulatorHelper emu; private long result; private float[] tracePos,traceNormal;
    private Address a(long p){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(p);}
    private void w(long p,int v)throws Exception{emu.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
    private int r(long p)throws Exception{byte[]b=emu.readMemory(a(p),4);return(b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);}
    private float f(long p)throws Exception{return Float.intBitsToFloat(r(p));}
    private void wf(long p,float v)throws Exception{w(p,Float.floatToRawIntBits(v));}
    private String bits(float x){return String.format("%08x",Float.floatToRawIntBits(x));}
    private String vec(float[]v){return "["+v[0]+","+v[1]+","+v[2]+"]";}
    private String vb(float[]v){return "[\""+bits(v[0])+"\",\""+bits(v[1])+"\",\""+bits(v[2])+"\"]";}
    private void push(long sp,int v)throws Exception{w(sp,v);}
    private void returnFromCall(int argc,long eax)throws Exception{long sp=emu.readRegister("ESP").longValue();int target=r(sp);emu.writeRegister("EAX",eax);emu.writeRegister("ESP",sp+4+argc);emu.getEmulator().setExecuteAddress(Integer.toUnsignedLong(target));}
    private int steps;
    private String sample(String name,boolean active,int world8,int worldB4,boolean collision)throws Exception{
        emu=new EmulatorHelper(currentProgram); result=collision?1:0;steps=0;
        float[][] inputs={{1.25f,-2.5f,3.75f},{-6,-6,-6},{6,6,6},{0.1f,0.2f,0.3f}};
        long[] ptr={INPUT,INPUT+12,INPUT+24,INPUT+36,INPUT+48,INPUT+60};
        emu.writeMemory(a(MODEL),new byte[0x200]);emu.writeMemory(a(INPUT),new byte[0x100]);emu.writeMemory(a(STACK-0x100),new byte[0x300]);
        w(MODEL+0x80,world8);w(MODEL+0x12c,worldB4);emu.writeMemory(a(MODEL+0x160),new byte[]{(byte)(active?1:0)});
        for(int i=0;i<4;i++)for(int j=0;j<3;j++)wf(ptr[i]+j*4,inputs[i][j]);
        for(int i=4;i<6;i++)for(int j=0;j<3;j++)wf(ptr[i]+j*4,99.0f+i+j);
        long sp=STACK;push(sp,(int)RETURN);for(long p:ptr)push(sp+=4,(int)p);
        emu.writeRegister("ESP",STACK);emu.writeRegister("ECX",MODEL);emu.writeRegister("EFLAGS",0x202);emu.writeRegister("DF",0);
        for(String reg:new String[]{"EAX","EBX","EDX","ESI","EDI","EBP"})emu.writeRegister(reg,0);
        emu.writeRegister("FPUControlWord",0x037f);emu.writeRegister("FPUStatusWord",0);emu.writeRegister("FPUTagWord",0xffff);
        for(String reg:new String[]{"ST0","ST1","ST2","ST3","ST4","ST5","ST6","ST7"})emu.writeRegister(reg,BigInteger.ZERO);
        emu.setBreakpoint(a(RETURN));emu.getEmulator().setExecuteAddress(ENTRY);
        long[] boundary=new long[11];tracePos=new float[3];traceNormal=new float[3];boolean entered=false;
        while(steps++<4000&&!emu.getExecutionAddress().equals(a(RETURN))){
            long pc=emu.getExecutionAddress().getOffset();
            if(pc==CALL){
                entered=true;long callSp=emu.readRegister("ESP").longValue();for(int i=0;i<11;i++)boundary[i]=Integer.toUnsignedLong(r(callSp+4L*i));
                long trace=boundary[10];tracePos=new float[]{4.25f,5.5f,-6.75f};traceNormal=new float[]{0,1,0};
                if(result!=0){for(int i=0;i<3;i++){wf(trace+12+i*4,tracePos[i]);wf(trace+28+i*4,traceNormal[i]);}}
                w(callSp-4,0x00492da6);emu.writeRegister("ESP",callSp-4);
                returnFromCall(0,result);continue;
            }
            if(!emu.step(monitor))throw new IllegalStateException("P-code error at "+emu.getExecutionAddress()+": "+emu.getLastError());
        }
        if(!emu.getExecutionAddress().equals(a(RETURN)))throw new IllegalStateException("Instruction limit at "+emu.getExecutionAddress());
        float[]out1={f(INPUT+48),f(INPUT+52),f(INPUT+56)},out2={f(INPUT+60),f(INPUT+64),f(INPUT+68)};
        long world=active?worldB4:world8;
        return "{\"name\":\""+name+"\",\"receiver\":"+MODEL+",\"active\":"+active+",\"selected_world\":"+world+",\"inputs\":["+vec(inputs[0])+","+vec(inputs[1])+","+vec(inputs[2])+","+vec(inputs[3])+"],\"input_addresses\":["+ptr[0]+","+ptr[1]+","+ptr[2]+","+ptr[3]+"],\"output_addresses\":["+ptr[4]+","+ptr[5]+"],\"collision_boundary_entered\":"+entered+",\"boundary_arguments\":["+longs(boundary)+"],\"boundary_vectors\":["+(entered?vec(readVec(boundary[1])):"[]")+","+(entered?vec(readVec(boundary[2])):"[]")+","+(entered?vec(readVec(boundary[3])):"[]")+","+(entered?vec(readVec(boundary[4])):"[]")+"],\"boundary_mask\":"+(entered?boundary[5]:0)+",\"boundary_type\":"+(entered?boundary[6]:0)+",\"hit\":"+((emu.readRegister("EAX").intValue()&0xff)!=0)+",\"outputs_written\":"+((emu.readRegister("EAX").intValue()&0xff)!=0)+",\"position\":"+vec(out1)+",\"normal\":"+vec(out2)+",\"position_bits\":"+vb(out1)+",\"normal_bits\":"+vb(out2)+",\"expected_engine_position\":"+vec(tracePos)+",\"expected_engine_normal\":"+vec(traceNormal)+",\"steps\":"+steps+"}";
    }
    private float[] readVec(long p)throws Exception{return new float[]{f(p),f(p+4),f(p+8)};}
    private String longs(long[]v){List<String>x=new ArrayList<>();for(long n:v)x.add(Long.toUnsignedString(n));return String.join(",",x);}
    @Override public void run()throws Exception{
        String[]args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: AxeWorldCollisionPcodeOracle.java output.json original.exe");
        StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
        List<String>rows=new ArrayList<>();rows.add(sample("inactive_world_hit",false,0x11112222,0x33334444,true));rows.add(sample("active_world_miss",true,0x11112222,0x33334444,false));rows.add(sample("selected_world_null",false,0,0x33334444,true));
        Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"entry\":\"00492CE0\",\"engine_boundary\":\"geWorld_Collision indirect call at 00492DA0; oracle returns controlled EAX and writes trace vectors only on hit\",\"scope\":\"Original wrapper instructions execute. The imported Genesis collision service is intercepted; this fixture verifies pointer selection, vector preparation, mask/type/flags, output-copy behavior and return value, not collision geometry or engine trace production.\",\"samples\":["+String.join(",",rows)+"]}\n");println("Captured "+rows.size()+" original Axe world-collision wrapper cases");
    }
}
