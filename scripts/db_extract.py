
import sys
import csv
import sqlite3
import os

def dump_db(filename):
    con = sqlite3.connect(filename)
    cur = con.cursor()
    res = (filename,) + cur.execute(getQuery(tablename)).fetchall()[0]
    csv.writerow(res)
    con.close()


def getQuery(base_tbl):
    return f"""SELECT t1.Experiment_params_VNRArrivalDist,
t1.Experiment_params_VNRArrivalDistParam1, 
t1.Experiment_params_VNRDurationDist, 
t1.Experiment_params_VNRDurationDistParam1, 
SUM (CASE WHEN t2.item_event_type = 'EMBD_SUCCESS' OR t2.item_event_type = 'EMBD_FAIL' THEN 1 ELSE 0 END), 
SUM (CASE WHEN t2.item_event_type = 'EMBD_SUCCESS' THEN 1 ELSE 0 END), 
SUM (CASE WHEN t2.item_event_type = 'EMBD_SUCCESS' THEN 1 ELSE 0 END)*1.0/SUM (CASE WHEN t2.item_event_type = 'EMBD_SUCCESS' OR t2.item_event_type = 'EMBD_FAIL' THEN 1 ELSE 0 END)*1.0, 
AVG(CASE WHEN t2.item_event_type = 'EMBD_SUCCESS' THEN t2.item_node_revenue END)+AVG(CASE WHEN t2.item_event_type = 'EMBD_SUCCESS' THEN t2.item_link_revenue END),
AVG(CASE WHEN t2.item_event_type = 'EMBD_SUCCESS' THEN t2.item_node_cost END)+AVG(CASE WHEN t2.item_event_type = 'EMBD_SUCCESS' THEN t2.item_link_cost END), 
AVG(CASE WHEN t2.item_event_type = 'EMBD_SUCCESS' THEN t2.item_avg_node_stress END), 
AVG(CASE WHEN t2.item_event_type = 'EMBD_SUCCESS' THEN t2.item_avg_link_stress END), 
AVG(CASE WHEN t2.item_event_type = 'EMBD_SUCCESS' THEN t2.item_actual_processing_time END)
FROM 
	{base_tbl} AS t1 
LEFT JOIN 
	{base_tbl}_statistics_items as t2 
ON 
	t1.hiberlite_id = t2.hiberlite_parent_id;"""


db_data = { "grc_bfs": "GRCNodeBFSLink",
            "grc_mcf": "GRCNodeMCFLink",
            "mcvne_bfs_bfs": "MCVNENodeBFSLink",
            "mcvne_bfs_mcf": "MCVNENodeMCFLink",
            "mcvne_mcf_mcf": "MCVNENodeMCFLink",
            "dvine_mcf": "VineNodeMCFLink",
            "rvine_mcf": "VineNodeMCFLink" }

csv = csv.writer(sys.stdout)

if len(sys.argv) != 3:
    print("Wrong number of arguments")
    print("db_extract algo filename")
    print("where algo is one of: grc_bfs, grc_mcf, mcvne_bfs_bfs mcvne_bfs_mcf mcvne_mcf_mcf dvine_mcf rvine_mcf")
    sys.exit(-1)

filename = sys.argv[2]
algo = sys.argv[1]

tablename = db_data[algo]

hdr = ["Algorithm", "Arrival_Dist", "Mean_Arrival_Rate", "Duration_Dist", "Mean_Duration", "N_VNRs", "N_Embedded_VNRs", "Acceptance_Ratio", "Avg_Rev", "Avg_Cost", "Avg_Node_Util", "Avg_Link_Util", "Avg_Proc_Time"]
csv.writerow(hdr)
if os.path.isdir(filename):
    for d in range(10, 81, 10):
        db_file = os.path.join(filename, "%s" % d, "Results")
        file = os.listdir(db_file)
        if len(file) != 1:
            print("Error: wrong file structure layout for output db.")
            sys.exit(-1)
        dump_db(os.path.join(filename, "%s" % d, "Results", file[0]))
else:
    dump_db(filename)




