
from unidecode import unidecode
import string  
    
sample_files = ['allsamples.txt', 'trainingsamples.txt', 'testingsamples.txt']
for filename in sample_files:
    
    # Process file. Remove punctuation, turn to lower case, and turn characters to ASCII
    with open(filename) as sf:
        
        file_text = ''.join(sf.readlines())
        file_text = file_text.lower()
        file_text = unidecode(file_text)
        file_text = ''.join([ch for ch in file_text if ch not in string.punctuation or ch == '-'])
        file_text = file_text.replace('-', ' ')
    
    # Write text back to file
    with open(filename, 'w') as sf:
        
        sf.write(file_text)
        
        