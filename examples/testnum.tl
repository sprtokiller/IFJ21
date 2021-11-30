require "ifj21"


function a(a:integer, b:boolean):integer, boolean
	return 5, ""
end

function s(a:integer, b:boolean):integer
	return a(5, true)
end

function main():string, boolean
	local y:string = ""
	local a:integer = 20
	local b:number
	local c:boolean = true
	
	b,c = s(s(a, c),c)

end
