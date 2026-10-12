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

/** Execute original 0047F5A0 cached-quad construction with affine frames. */
public class AshParticleGeometryPcodeOracle extends GhidraScript {
    private static final long PARTICLE=0x20000000L, STACK=0x20fff000L, RETURN=0x70000000L;
    private static final long BUILD=0x0047f5a0L, GUARD=0x005030bcL;
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
        e.getEmulator().setExecuteAddress(BUILD);
        for(int i=0;i<100000&&e.getExecutionAddress().getOffset()!=RETURN;i++)
            if(!e.step(monitor))throw new IllegalStateException("Geometry P-code error at "+e.getExecutionAddress()+": "+e.getLastError());
        if(e.getExecutionAddress().getOffset()!=RETURN)
            throw new IllegalStateException("Geometry instruction limit at "+e.getExecutionAddress());
    }
    private String sample(String name,int[] size,int[] axes,int[] origin,int[] scale)throws Exception{
        EmulatorHelper e=new EmulatorHelper(currentProgram);
        try{
            e.writeMemory(a(PARTICLE),new byte[0x200]);e.writeMemory(a(STACK),new byte[0x100]);
            w(e,PARTICLE+0x34,0x004dfc30);long cursor=PARTICLE+0x38;
            for(int word:axes){w(e,cursor,word);cursor+=4;}
            for(int word:origin){w(e,cursor,word);cursor+=4;}
            for(int word:scale){w(e,cursor,word);cursor+=4;}
            w(e,PARTICLE+0x74,0);w(e,PARTICLE+0x78,1);
            for(int i=0;i<3;i++)w(e,PARTICLE+0xb4+4*i,size[i]);
            w(e,GUARD,1);zeroCpu(e);w(e,STACK,(int)RETURN);e.writeRegister("ESP",STACK);e.writeRegister("ECX",PARTICLE);
            run(e);int[] offsets={0xd8,0xfc,0x120,0x144};List<String> vertices=new ArrayList<>();
            for(int offset:offsets)vertices.add("["+Integer.toUnsignedLong(r(e,PARTICLE+offset))+","+Integer.toUnsignedLong(r(e,PARTICLE+offset+4))+","+Integer.toUnsignedLong(r(e,PARTICLE+offset+8))+ "]");
            return "{\"name\":\""+name+"\",\"size_before\":["+Integer.toUnsignedLong(size[0])+","+Integer.toUnsignedLong(size[1])+","+Integer.toUnsignedLong(size[2])+
                "],\"axes_before\":["+join(axes)+"],\"origin_before\":["+join(origin)+"],\"scale_before\":["+join(scale)+"],\"vertices_after\":["+String.join(",",vertices)+"],\"geometry_dirty_after\":"+(e.readMemory(a(PARTICLE+0x78),1)[0]!=0)+"}";
        }finally{e.dispose();}
    }
    private String join(int[] values){List<String> words=new ArrayList<>();for(int x:values)words.add(Integer.toUnsignedString(x));return String.join(",",words);}
    @Override public void run()throws Exception{
        String[] args=getScriptArgs();if(args.length!=2)throw new IllegalArgumentException("Usage: AshParticleGeometryPcodeOracle.java output.json original.exe");
        StringBuilder hash=new StringBuilder();for(byte b:MessageDigest.getInstance("SHA-256").digest(Files.readAllBytes(Path.of(args[1]))))hash.append(String.format("%02x",b));
        if(!hash.toString().equalsIgnoreCase(currentProgram.getExecutableSHA256()))throw new IllegalStateException("Executable hash mismatch");
        int[][] sizes={{0x3f800000,0x3f800000,0x3f800000},{0x40000000,0x3f000000,0x40400000},{0x3f000000,0x40800000,0x3f800000},{0,0x80000000,0x3f000000},{0x3f800001,0x3f000001,0x40000001}};
        int[][] axes={
            {0x3f800000,0,0,0,0x3f800000,0,0,0,0x3f800000},
            {0,0,0xbf800000,0,0x3f800000,0,0x3f800000,0,0},
            {0x3f800000,0,0,0,0,0xbf800000,0,0x3f800000,0},
            {0xbf800000,0,0,0,0x3f800000,0,0,0,0x3f800000},
            {0x3f4ccccd,0,0x3f19999a,0,0x3f800000,0,0xbf19999a,0,0x3f4ccccd}
        };
        int[][] origins={{0,0,0},{0x41200000,0x41a00000,0xc1f00000},{0xc1200000,0x3f800000,0x42c80000},{0x80000000,0x3f000000,0xbf000000},{0x3eaaaaab,0xc1200000,0x42a00000}};
        int[][] scales={{0x3f800000,0x3f800000,0x3f800000},{0x40000000,0x3f000000,0x3fc00000},{0x3f800000,0x3f000000,0x40000000},{0x3f000000,0x40000000,0x3f800000},{0x3f800001,0x3f000001,0x3fc00001}};
        List<String> rows=new ArrayList<>();for(int i=0;i<sizes.length;i++)rows.add(sample("geometry_"+i,sizes[i],axes[i],origins[i],scales[i]));
        Files.writeString(Path.of(args[0]),"{\"exe_sha256\":\""+hash+"\",\"engine\":\"Ghidra EmulatorHelper original x86 PE32 P-code\",\"entry\":\"0047F5A0\",\"scope\":\"Original four cached position vertices through 004977B0 and 00495D20; no parent transform; local static initializer is marked already run; other render fields and polygon submission are outside scope\",\"samples\":["+String.join(",",rows)+"]}\n");
        println("Captured "+rows.size()+" original ash geometry cases");
    }
}
