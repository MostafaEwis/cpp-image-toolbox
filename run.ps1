Param([Parameter(Mandatory)]$Path, $c)
IF($c -eq 't'){
	Write-Host "compiling..."
	g++ .\loadimage.cpp .\image.cpp -lopengl32 -lglut32win -lglu32
	Write-Host "finished"
}
./a.exe $Path
