
sample_files = ['trainingsamples.txt']
for filename in sample_files:
    
    # Process file. Remove punctuation, turn to lower case, and turn characters to ASCII
    with open(filename) as sf:
        
        file_text = sf.readlines()
        new_file_text = ""
        for i, line in enumerate(file_text):
            if i % 2 != 0:
                line = line[:-1] + ' '
                print(line)
            new_file_text += line 
    
    # Write text back to file
    with open(filename, 'w') as sf:
        
        sf.write(new_file_text)