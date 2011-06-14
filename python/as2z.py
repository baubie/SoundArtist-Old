import sys

def as2z(file):
        TXTFILE = open(file+".txt","r")
        data = []
        for line in TXTFILE:
                data.append(line.split("\n")[0].split("\t"))
        TXTFILE.close()
        data = [[r[col].replace("\n","") for r in data] for col in range(1,len(data[0]))]

        ZFILE = open(file+".z", "w")
        ZFILE.write("! nx "+str(len(data[0])-1))
        ZFILE.write(" ny "+str(len(data[:-1])))
        ZFILE.write(" xmin 0")
        ZFILE.write(" xmax 800")
        ZFILE.write(" ymin 0")
        ZFILE.write(" ymax 150")
        ZFILE.write("\n")
        for line in data[:-1]:
                out = ''
                count = 0
                for value in line[1:]:
                        if str.isdigit(value):
                                out = out + str(int(value.replace("\n",""))) + " "
                                count = count + 1
                print >>ZFILE, out[:-1]
        ZFILE.close()
# END OF as2z


as2z(sys.argv[1])
