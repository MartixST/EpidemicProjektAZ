CC = gcc

main = epidem

data = real_data.csv

output = output.html

linux: 
	$(CC) -lm -fdiagnostics-color=always -g $(main).c -o $(main)
	./$(main) $(data) 8 0.9 $(output)
	xdg-open $(output)

windows: 
	$(CC) -lm -fdiagnostics-color=always -g $(main).c -o $(main)
	./$(main).exe $(data) 8 0.9 $(output)