
strgindex = []

def findatg():
    chrx = open("/Users/Thor/Desktop/chrX.fa").read().upper().replace("N","").replace("\n","").replace(" ","")
    index_count = 0
    new_index = 0
    while new_index != -1:
        new_index = chrx.find("TATAAA",index_count)
        atg = chrx.find("ATG",index_count+21,index_count+36)
        index_count = new_index + 1
        if atg != -1:
            TAG_stop = chrx.find("TAG",atg)
            TAA_stop = chrx.find("TAA",atg)
            TGA_stop = chrx.find("TGA",atg)
            if TAG_stop < TAA_stop and TAG_stop < TGA_stop and TAG_stop > atg:
                a = chrx[atg:TAG_stop+3]
                if len(a) > 5:       
                    strgindex.append(a)
            elif TAA_stop < TGA_stop and TAA_stop < TAG_stop and TAA_stop > atg:
                a = chrx[atg:TAA_stop+3]
                if len(a) > 5:
                    strgindex.append(a)
            elif TGA_stop < TAA_stop and TGA_stop < TAG_stop and TGA_stop > atg:
                a = chrx[atg:TGA_stop+3]
                if len(a) > 5:    
                    strgindex.append(a)
                
findatg()

acidlist = []

class Acid:
    """ Represents an Amino acid """ 
    def __init__(self,abbreviation,shortname,longname):
        self.abbreviation = abbreviation
        self.shortname = shortname
        self.longname = longname
        self.aminolist = []
    
    def __str__(self):
        return '%s %s %s' %(self.abbreviation,self.shortname,self.longname) 

def acid_creater():
    with open("/users/thor/desktop/dna-codons.cdl") as fin:
        for line in fin:
            if line[0] != ">":
                b = line.strip()
                if len(b) > 0:
                    acid.aminolist.append(line.strip())
            else:
                a = line.replace(">","").split(",")
                acid = Acid(a[0],a[1],a[2])
                acidlist.append(acid)    
acid_creater()


class Ribosome:
    """ stores mappings from amino acids to codons in dictionaries and then reverses them in new dictionaries """
    
    def __init__(self):
        self.d_abbr = dict() 
        self.d_short = dict()
        self.d_long = dict()
        
        self.d_abbr_inv = dict()
        self.d_short_inv = dict()
        self.d_long_inv = dict()
        
        for i in acidlist:
            self.d_abbr[i.abbreviation.strip()] = (i.aminolist)
            self.d_short[i.shortname.strip()] = (i.aminolist)
            self.d_long[i.longname.strip()] = (i.aminolist)
        
        for k in self.d_abbr:
            for v in self.d_abbr[k]:
                self.d_abbr_inv.setdefault(v,k)
        for k in self.d_short:
            for v in self.d_short[k]:
                self.d_short_inv.setdefault(v,k)
        for k in self.d_long:
            for v in self.d_long[k]:
                self.d_long_inv.setdefault(v,k)
                
    def printer(self,a):
        """Able to search the inverted dicts with three or more codon"""
        print(self.d_abbr_inv[a])
        print(self.d_short_inv[a])
        print(self.d_long_inv[a])
        


class Protean(Ribosome):
    
    def __init__(self):
        Ribosome.__init__(self)


    def translator(self,parameter,codon):
        #parameter can be 1 for abbreviation, 2 for the short name or 3 for the long name 
        if parameter == 1:
            dict = self.d_abbr_inv
        elif parameter == 2:
            dict = self.d_short_inv
        elif parameter == 3:
            dict = self.d_long_inv
        line = codon
        n = 3 
        a = [line[i:i+n] for i in range(0, len(line), n)]
        for i in a:
            if len(i) > 2:
                print(dict[i])
            else:
                continue
                
    def codon_tester(self,parameter):
        for i in strgindex:
            print(i)
            i = i[:-1]
            print(i)
            print(protean.translator(parameter,i))



protean = Protean()

