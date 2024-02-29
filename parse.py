import re




'''
    Loads variables from file, also formats them in regex compatible style

'''
def load_variables():
    file = open("variables.txt", "r")
    ret_list = []
    lines = file.readlines()
    for line in lines:
        line = line.strip()
        if line != "":
            formated_line = f'^;{line}:.*'
            ret_list.append(formated_line)
            #print(formated_line)
    return ret_list




def main():
    file = open("example.gcode", "r")
    lines = file.readlines()
    variables = load_variables()
    for line in lines:
        for var in variables:
            x = re.search(var, line)
            if x != None:
                print(f'Match found: {x}')



    file.close()


if __name__ == "__main__":
    main()
