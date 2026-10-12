import ghidra.app.emulator.EmulatorHelper;
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.lang.Register;
import java.math.BigInteger;
import java.nio.file.Files;
import java.nio.file.Path;
import java.security.MessageDigest;
import java.util.ArrayList;
import java.util.List;

/** Execute original 004977B0 parent-chain resolution for fixed transform trees. */
public class AshTransformChainPcodeOracle extends GhidraScript {
    private static final long STACK=0x20fff000L, RETURN=0x70000000L;
    private static final long NODES=0x21000000L, OUTPUT=0x21001000L;
    private static final long RESOLVE=0x004977b0L;
    private Address a(long p){return currentProgram.getAddressFactory().getDefaultAddressSpace().getAddress(p);}
    private void w(EmulatorHelper e,long p,int v)throws Exception{e.writeMemoryValue(a(p),4,Integer.toUnsignedLong(v));}
    private int r(EmulatorHelper e,long p)throws Exception{
        byte[] b=e.readMemory(a(p),4);
        return (b[0]&255)|((b[1]&255)<<8)|((b[2]&255)<<16)|((b[3]&255)<<24);
    }
    private void zeroCpu(EmulatorHelper e)throws Exception{
        for(String n:new String[]{"EAX","EBX","ECX","EDX","ESI","EDI","EBP"})e.writeRegister(n,0);
        e.writeRegister("DF",0);e.writeRegister("EFLAGS",0x202);
        Register cw=currentProgram.getLanguage().getRegister("FPUControlWord");if(cw!=null)e.writeRegister(cw,0x037fL);
        Register sw=currentProgram.getLanguage().getRegister("FPUStatusWord");if(sw!=null)e.writeRegister(sw,0);
        Register tag=currentProgram.getLanguage().getRegister("FPUTagWord");if(tag!=null)e.writeRegister(tag,0xffffL);
        for(Register reg:currentProgram.getLanguage().getRegisters())
            if(reg.getName().toUpperCase().matches("ST[0-7]"))e.writeRegister(reg,BigInteger.ZERO);
    }
    private void run(EmulatorHelper e)throws Exception{
        e.getEmulator().setExecuteAddress(RESOLVE);
        for(int i=0;i<200000&&e.getExecutionAddress().getOffset()!=RETURN;i++)
            if(!e.step(monitor))throw new IllegalStateException("Ash transform P-code error at "+e.getExecutionAddress()+": "+e.getLastError());
        if(e.getExecutionAddress().getOffset()!=RETURN)
            throw new IllegalStateException("Ash transform instruction limit at "+e.getExecutionAddress());
    }
    private String join(EmulatorHelper e,long address,int words)throws Exception{
        List<String> values=new ArrayList<>();for(int i=0;i<words;i++)values.add(Integer.toUnsignedString(r(e,address+4L*i)));
        return String.join(",",values);
    }
    private static final class Sample {
        final String name; final int[][] axes,origin,scale; final int[] parent;
        Sample(String n,int[][] a,int[][] o,int[][] s,int[] p){name=n;axes=a;origin=o;scale=s;parent=p;}
    }
    private String sample(Sample s)throws Exception{
        EmulatorHelper e=new EmulatorHelper(currentProgram);
        try{
            e.writeMemory(a(STACK),new byte[0x200]);e.writeMemory(a(OUTPUT),new byte[0x80]);
            for(int i=0;i<s.parent.length;i++){
                long node=NODES+0x100L*i;
                e.writeMemory(a(node),new byte[0x80]);w(e,node,0x004dfc30);
                for(int j=0;j<9;j++)w(e,node+4+4L*j,s.axes[i][j]);
                for(int j=0;j<3;j++)w(e,node+0x28+4L*j,s.origin[i][j]);
                for(int j=0;j<3;j++)w(e,node+0x34+4L*j,s.scale[i][j]);
                w(e,node+0x40,s.parent[i]<0?0:(int)(NODES+0x100L*s.parent[i]));
            }
            zeroCpu(e);w(e,STACK,(int)RETURN);w(e,STACK+4,(int)OUTPUT);
            w(e,STACK+8,(int)(NODES+0x100L*(s.parent.length-1)));e.writeRegister("ESP",STACK);
            run(e);
            return "{\"name\":\""+s.name+"\",\"parents\":["+joinInts(s.parent)+"],\"nodes\":["+inputNodes(s)+"],\"resolved_axes\":["+join(e,OUTPUT+4,9)+"],\"resolved_origin\":["+join(e,OUTPUT+0x28,3)+"],\"resolved_scale\":["+join(e,OUTPUT+0x34,3)+"],\"resolved_parent\":"+Integer.toUnsignedString(r(e,OUTPUT+0x40))+"}";
        }finally{e.dispose();}
    }
    private String joinInts(int[] v){List<String>x=new ArrayList<>();for(int a:v)x.add(Integer.toString(a));return String.join(",",x);}
    private String inputNodes(Sample s){
        List<String> nodes=new ArrayList<>();
        for(int i=0;i<s.parent.length;i++)nodes.add("{\"parent\":"+s.parent[i]+",\"axes\":["+join(s.axes[i])+"],\"origin\":["+join(s.origin[i])+"],\"scale\":["+join(s.scale[i])+"]}");
        return String.join(",",nodes);
    }
    private String join(int[] v){List<String>x=new ArrayList<>();for(int a:v)x.add(Integer.toUnsignedString(a));return String.join(",",x);}
    @Override public void run()throws Exception{
        String[] args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: AshTransformChainPcodeOracle.java output.json original.exe");
        StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
        if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
        int[][] a0={{0x3f800000,0,0,0,0x3f800000,0,0,0,0x3f800000}};
        int[][] o0={{0x3eaaaaab,0xc1200000,0x42a00000}},s0={{0x3f800001,0x3f000001,0x3fc00001}};
        int[][] a1={{0x3f3504f3,0x3f3504f3,0,0xbf3504f3,0x3f3504f3,0,0,0,0x3f800000},{0x3f800000,0x3e800000,0xbf000000,0xbf400000,0x3f000000,0x3e000000,0x3eaaaaab,0xbf19999a,0x3f666666}};
        int[][] o1={{0x41200000,0xc1200000,0x41a00000},{0x3f000001,0x40000001,0xc0400001}},s1={{0x40000000,0x3f000000,0x3fc00000},{0x3f400000,0x3f800001,0x3eaaaaab}};
        int[][] a2={{0x3f000000,0,0x3f000000,0,0x3f800000,0,0xbf000000,0,0x3f000000},{0x3f666666,0x3e4ccccd,0,0xbf19999a,0x3f666666,0x3e99999a,0x3dcccccd,0xbf000000,0x3f800000},{0x3f800001,0xbf000001,0x3e800001,0x3e000001,0x3f400001,0xbf600001,0xbf200001,0x3eaaaaab,0x3f19999a}};
        int[][] o2={{0x41200000,0x41a00000,0xc1f00000},{0xc1200000,0x3f800000,0x42c80000},{0x3eaaaaab,0xc1200000,0x42a00000}},s2={{0x40000000,0x3f800000,0x3f000000},{0x3f000000,0x3f800000,0x40000000},{0x3f800001,0x3f000001,0x3fc00001}};
        int[][] a3={{0xbf800000,0,0,0,0x3f800000,0,0,0,0x3f800000},{0x3f800000,0,0,0,0x3f800000,0,0,0,0x3f800000}};
        int[][] o3={{0x80000000,0x3f000000,0xbf000000},{0,0x80000000,0x3f000000}},s3={{0x3f000000,0x40000000,0x3f800000},{0x40000000,0x3f000000,0x3f800000}};
        Sample[] samples={
            new Sample("transform_root_only",a0,o0,s0,new int[]{-1}),
            new Sample("transform_one_parent",a1,o1,s1,new int[]{-1,0}),
            new Sample("transform_two_parent_levels",a2,o2,s2,new int[]{-1,0,1}),
            new Sample("transform_signed_zero",a3,o3,s3,new int[]{-1,0})
        };
        List<String> rows=new ArrayList<>();for(Sample s:samples)rows.add(sample(s));
        Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"004977B0\",\"scope\":\"Resolved Genesis transform fields through 00497300, 00495D20, and 0042D0A0; parent chains are bounded to authored acyclic nodes; no Training runtime parent sample\",\"samples\":["+String.join(",",rows)+"]}\n");
        println("Captured "+rows.size()+" original ash transform-chain cases");
    }
}
