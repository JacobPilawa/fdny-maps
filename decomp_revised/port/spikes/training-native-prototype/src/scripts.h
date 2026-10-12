#pragma once
#include <deque>
#include <sstream>
// Interpreter for the Training EVS commands, using the preserved scripts verbatim.
inline std::string trim(std::string s){auto a=s.find_first_not_of(" \t\r\n");if(a==std::string::npos)return "";auto b=s.find_last_not_of(" \t\r\n");return s.substr(a,b-a+1);}
inline std::string unwrap(std::string s){s=trim(s);while(s.size()>1&&((s.front()=='('&&s.back()==')')||(s.front()=='['&&s.back()==']'))){int depth=0;bool whole=true;for(size_t i=0;i<s.size()-1;i++){if(s[i]=='('||s[i]=='[')depth++;if(s[i]==')'||s[i]==']')depth--;if(!depth){whole=false;break;}}if(!whole)break;s=trim(s.substr(1,s.size()-2));}return s;}
inline std::vector<std::string> splitList(std::string s){std::vector<std::string> out;int depth=0;size_t start=0;for(size_t i=0;i<s.size();i++){if(s[i]=='('||s[i]=='[')depth++;if(s[i]==')'||s[i]==']')depth--;if(s[i]==','&&!depth){out.push_back(trim(s.substr(start,i-start)));start=i+1;}}out.push_back(trim(s.substr(start)));return out;}
inline std::pair<std::string,std::string> token(std::string s){s=trim(s);size_t i=0;if(!s.empty()&&(s[0]=='('||s[0]=='[')){int depth=0;for(;i<s.size();i++){if(s[i]=='('||s[i]=='[')depth++;if(s[i]==')'||s[i]==']')depth--;if(!depth){i++;break;}}}else{i=s.find_first_of(" \t\r\n");if(i==std::string::npos)i=s.size();}return {s.substr(0,i),trim(s.substr(i))};}
struct Scripts{
 struct Queued{int entity;std::string command,source;bool startup=false;};
 struct ActorQueue{std::deque<Queued> commands;uint32_t waitUntil=0;bool waiting=false;};
 struct Binding{std::string event,command,source;};
 uint32_t timeMs=0;bool definitionsOnly=false;std::map<std::string,std::string> variables;std::map<std::string,std::string> files;std::map<int,std::vector<Binding>> bindings;std::map<int,ActorQueue> actorQueues;std::set<std::string> once,reported;
 std::function<void(std::string,float)> hint;
 std::function<bool(int,std::string)> host;
 std::function<int(std::string)> findEntity;
 explicit Scripts(J bundle){auto sources=field(bundle,"sources");for(int i=0;i<count(sources);i++){auto source=at(sources,i);auto path=str(field(source,"path"));files[path.substr(path.find_last_of('/')+1)]=str(field(source,"text"));}variables["LEVEL"]="training";variables["DIFFICULTY"]="1";}
 void report(std::string command,std::string source){auto key=source+":"+command;if(reported.insert(key).second)std::cerr<<"Unimplemented EVS: "<<key<<"\n";}
 // 00471310 -> 0047A130 -> 004799E0: the early file pass admits only
 // top-level set/exec lines. Nested exec files inherit this mode.
 void loadLevel(std::string file,int entity,bool definitions){bool previous=definitionsOnly;definitionsOnly=definitions;try{run("exec "+file,entity,"level",0,true);}catch(...){definitionsOnly=previous;throw;}definitionsOnly=previous;}
 void run(std::string raw,int entity,std::string source,int depth=0,bool startup=false){
 auto command=unwrap(raw);if(command.empty()||command=="<null>")return;if(depth>64){report(command,source);return;}
 auto [verb,args]=token(command);auto colon=verb.find(':');if(colon!=std::string::npos){std::string event=verb.substr(0,colon);if(startup)bindings[entity].push_back({event,args,source});else run(args,entity,source+"/event",depth+1);return;}
 if(verb=="set"){auto [name,value]=token(args);variables[name]=unwrap(value);return;}
 if(verb=="exec"){auto file=files.find(args);if(file==files.end()){report(command,source);return;}std::istringstream lines(file->second);std::string line;int number=0;while(std::getline(lines,line)){number++;auto text=trim(line);if(text.rfind("//",0)==0||text.rfind(";",0)==0)continue;auto first=token(text).first;if(definitionsOnly&&first!="set"&&first!="exec")continue;run(line,entity,args+":"+std::to_string(number),depth+1,true);}return;}
 if(verb.size()>1&&verb[0]=='*'){auto name=verb.substr(1);auto it=variables.find(name);if(it==variables.end())report(command,source);else run(it->second,entity,source+"/*"+name,depth+1,startup);return;}
 if(verb=="if"||verb=="ifnot"){auto [name,rest]=token(args);auto [wanted,body]=token(rest);auto value=variables.count(name)?variables[name]:"0";bool equal=false;for(auto& v:splitList(unwrap(wanted)))if(value==v)equal=true;if(equal==(verb=="if"))run(body,entity,source+"/condition",depth+1,startup);return;}
 if(verb=="once"){auto key=std::to_string(entity)+":"+source;if(once.insert(key).second)run(args,entity,source+"/once",depth+1,startup);return;}
 if(verb=="cmd"){int index=0;for(auto part:splitList(unwrap(args))){auto origin=source+"/"+std::to_string(index++);run(part,entity,origin,depth+1,startup);}return;}
 if(verb=="que"){int index=0;auto& queue=actorQueues[entity];for(auto part:splitList(unwrap(args))){auto origin=source+"/"+std::to_string(index++);queue.commands.push_back({entity,part,origin,startup});}return;}
 if(verb=="wait"){auto [seconds,rest]=token(args);(void)rest;auto milliseconds=uint32_t(std::stof(seconds)*1000.f);actorQueues[entity].waitUntil=timeMs+milliseconds;actorQueues[entity].waiting=true;return;}
 if(verb=="idle"){actorQueues[entity].waiting=false;return;}
 if(verb=="flush"){auto& queue=actorQueues[entity];queue.commands.clear();queue.waiting=false;return;}
 if(verb=="hint"){auto [seconds,text]=token(args);hint(unwrap(text),std::stof(seconds));return;}
 if(verb=="tell"||verb=="action"){auto [target,body]=token(args);int id=findEntity(target);if(id<0){report(command,source);return;}run(body,id,source+"/tell",depth+1,startup);return;}
 if(verb=="inc"||verb=="dec"){variables[args]=std::to_string(std::stoi(variables.count(args)?variables[args]:"0")+(verb=="inc"?1:-1));return;}
 if(!host(entity,command))report(command,source);
 }
 void dispatch(int entity,std::string event){auto copy=bindings[entity];for(auto& binding:copy)if(binding.event==event)run(binding.command,entity,binding.source);}
 void startupEntity(J e){int entity=integer(field(e,"index"));auto props=field(e,"properties");for(int i=1;i<=10;i++){char key[24];snprintf(key,sizeof(key),"S_Script%02d",i);run(str(field(props,key)),entity,std::to_string(entity)+":"+key,0,true);}}
 // 00470740 checks the absolute wait deadline and removes at most one queued
 // command per ScriptActor update. The per-entity primitive lets a reconstructed
 // owner callback preserve its local contact -> motion -> queue order without
 // claiming that the host's global callback schedule is original.
 void advanceEntity(int entity,uint32_t currentTimeMs){timeMs=currentTimeMs;auto found=actorQueues.find(entity);if(found==actorQueues.end())return;auto& queue=found->second;if(queue.waiting){if(queue.waitUntil>timeMs)return;queue.waiting=false;}if(queue.commands.empty())return;auto item=std::move(queue.commands.front());queue.commands.pop_front();run(item.command,item.entity,item.source,0,item.startup);}
 // The host currently approximates the original scheduler by visiting every
 // queued entity once per frame, in entity-ID order.
 void advance(uint32_t currentTimeMs){timeMs=currentTimeMs;std::vector<int> scheduled;for(const auto& [entity,queue]:actorQueues)scheduled.push_back(entity);for(int entity:scheduled)advanceEntity(entity,currentTimeMs);}
};
