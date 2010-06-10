var overload=require("../lib/overload");
var fs=require("fs");
var sys=require("sys");
var Buffer=require("buffer").Buffer;
sys.puts(process.argv)
var time

function CompileTo(src,path) {
	var CompileData=overload.Compile(fs.readFileSync("compilable.js"));
	//sys.puts(CompileData.byteLength())
	sys.puts(CompileData.length)
	fs.writeFileSync("compilable_data.jsc",CompileData);
}

function LoadFrom(path) {
	overload.Load(
		fs.readFileSync("compilable_data.jsc",'binary')
	)
}

if(process.argv[2]=="compile") {
	time=new Date().getTime();
	CompileTo(fs.readFileSync("compilable.js"),"compilable_data.jsc");
}
else if(process.argv[2]=="require") {
	time=new Date().getTime();
	var cat=require("./compilable").cat;
}
else if(process.argv[2]=="compiledload") {
	time=new Date().getTime();
}
else {
	CompileTo(fs.readFileSync("compilable.js"),"compilable_data.jsc");
	LoadFrom("compilable_data.jsc")
}
sys.puts("START:"+time)
if(typeof a!="undefined") sys.puts("CAT:"+JSON.stringify(a));
var newt=(new Date()).getTime()
sys.puts("End:"+(newt-time),newt,time)