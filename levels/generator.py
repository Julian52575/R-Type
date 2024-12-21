from random import randint
import sys

def make_entity(entities,nbr_entities):
    return {
        "position":{
            "x": 820,
            "y": randint(0, 550),
        },
        "entity": entities[randint(0, len(entities) - 1)],
        "spawntime": randint(0, nbr_entities),
    }

def make_level(entities,level_name,nbr_enitities):


    with open(f"levels/{level_name}.json", "w") as f:
        f.write("{\n")
        f.write('\t"entities": [\n')
        for i in range(nbr_enitities):
            entity = make_entity(entities,nbr_enitities)
            print(entity)
            string = str(entity).replace("'", '"') + ","
            if i == nbr_enitities-1:
                string = string[:-1]
            f.write("\t\t" + string + "\n")
        f.write("\t]\n")
        f.write("}\n")



if __name__ == "__main__":
    entities = ["entities/dragon_rouge.json", "entities/dragon_noir.json"]
    nbr_arg = len(sys.argv)
    if nbr_arg !=3:
        print("Generator.py [levelName] [number of entities]")
        exit (84)
    level_name = sys.argv[1]
    nbr = int(sys.argv[2])
    make_level(entities,level_name,nbr)
    