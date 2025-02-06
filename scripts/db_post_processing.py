
import sys
import csv
import sqlite3


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


#con = sqlite3.connect("grc_mcf_reqs-12-1000-nodesMin-3-nodesMax-10-grid-25.db")

#cur = con.cursor()

#res = cur.execute(query).fetchall()


db_data = { "grc_bfs":("grc_bfs_reqs-12-1000-nodesMin-3-nodesMax-10-grid-25.db", "GRCNodeBFSLink"),
            "grc_mcf":("grc_mcf_reqs-12-1000-nodesMin-3-nodesMax-10-grid-25.db", "GRCNodeMCFLink"),
            "mcvne_bfs_bfs":("mcvne_bfs_bfs_reqs-12-1000-nodesMin-3-nodesMax-10-grid-25.db", "MCVNENodeBFSLink"),
            "mcvne_bfs_mcf":("mcvne_bfs_mcf_reqs-12-1000-nodesMin-3-nodesMax-10-grid-25.db", "MCVNENodeMCFLink"),
            "mcvne_mcf_mcf":("mcvne_mcf_mcf_reqs-12-1000-nodesMin-3-nodesMax-10-grid-25.db", "MCVNENodeMCFLink"),
            "dvine_mcf":("vineyard_deterministic_reqs-12-1000-nodesMin-3-nodesMax-10-grid-25.db", "VineNodeMCFLink"),
            "rvine_mcf":("vineyard_randomized_reqs-12-1000-nodesMin-3-nodesMax-10-grid-25.db", "VineNodeMCFLink" )}

csv = csv.writer(sys.stdout)

hdr = ["Algorithm", "Arrival_Dist", "Mean_Arrival_Rate", "Duration_Dist", "Mean_Duration", "N_VNRs", "N_Embedded_VNRs", "Acceptance_Ratio", "Avg_Rev", "Avg_Cost", "Avg_Node_Util", "Avg_Link_Util", "Avg_Proc_Time"]
csv.writerow(hdr)

for db in [ "mcvne_mcf_mcf", "mcvne_bfs_mcf", "mcvne_bfs_bfs", "grc_mcf", "grc_bfs", "dvine_mcf", "rvine_mcf" ]:
    (filename, tablename) = db_data[db]

    con = sqlite3.connect(filename)
    cur = con.cursor()
    res = (db,) + cur.execute(getQuery(tablename)).fetchall()[0]
    csv.writerow(res)
    con.close()
    

    
