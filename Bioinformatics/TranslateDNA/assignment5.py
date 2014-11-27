__author__ = 'anglies'

ProteinDict = {"att": "I", "atc": "I", "ata": "I", "ctt": "L",
               "ctc": "L", "cta": "L", "ctg": "L", "tta": "L",
               "ttg": "L", "gtt": "V", "gtc": "V", "gta": "V",
               "gtg": "V", "ttt": "F", "ttc": "F", "atg": "Met",
               "tgt": "C", "tgc": "C", "gct": "A", "gcc": "A",
               "gca": "A", "gcg": "A", "ggt": "G", "ggc": "G",
               "gga": "G", "ggg": "G", "cct": "P", "ccc": "P",
               "cca": "P", "ccg": "P", "act": "T", "acc": "T",
               "aca": "T", "acg": "T", "tct": "S", "tcc": "S",
               "tca": "S", "tcg": "S", "agt": "S", "agc": "S",
               "tat": "Y", "tac": "UY", "tgg": "W", "caa": "Q",
               "cag": "Q", "aat": "N", "aac": "N", "cat": "H",
               "cac": "H", "gaa": "E", "gag": "E", "gat": "D",
               "gac": "D", "aaa": "K", "aag": "K", "cgt": "R",
               "cgc": "R", "cga": "R", "cgg": "R", "aga": "R",
               "agg": "R", "taa": "Stop", "tag": "Stop", "tga": "Stop"}


def IDprotsInDNA(dna):
    value = True

    i = 0
    while value and i + 2 < (len(dna)):
        if dna[i:i + 3] in ProteinDict:
            print ProteinDict[dna[i:i + 3]],
            i = i + 3
        else:
            value = False


def DNAToRNA(dna):
    dnaString = []
    for i in range(len(dna)):
        if dna[i] == "a":
            dnaString.append("t")

        if dna[i] == "c":
            dnaString.append("g")

        if dna[i] == "g":
            dnaString.append("c")

        if dna[i] == "t":
            dnaString.append("a")

    return "".join(dnaString)


if __name__ == "__main__":

    try:
        DNAString = None
        Gene = open("test.txt", "r")

        no_digit_result = ''.join([i for i in Gene.read() if not i.isdigit()])

        DNAString = no_digit_result.translate(None, ' \t\r\n')

        print
        print
        print DNAString
        print "5'3' Frame 1:"
        IDprotsInDNA(DNAString)
        print DNAString

        print
        print
        print
        print "5'3' Frame 2:"
        IDprotsInDNA(DNAString[1:])
        print
        print
        print
        print "5'3' Frame 3"
        IDprotsInDNA(DNAString[2:])
        reverseDNAString = DNAString[::-1]
        print
        print
        print
        print "3'5' Frame 1"
        IDprotsInDNA(DNAToRNA(reverseDNAString))

        print
        print""
        print reverseDNAString
        print
        print
        print
        print "3'5' Frame 2"
        IDprotsInDNA(DNAToRNA(reverseDNAString[1:]))
        print
        print
        print
        print "3'5' Frame 3"
        IDprotsInDNA(DNAToRNA(reverseDNAString[2:]))



    except IOError:
        print "File cannot be opened!"
