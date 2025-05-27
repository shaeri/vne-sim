import fnss
import networkx as nx
import numpy as np
from fnss.topologies.datacenter import DatacenterTopology


def butterfly_topology(k):

    b = nx.Graph()

    for i in range(0,2**k):
        b.add_node(i)

    for i in range(0,k):
        for j in range(0,2**k):
            b.add_node((i,j))

    for i in range(0,2**k):
        b.add_edge(i,(0,i))
        b.add_edge(i,(k,i))

    for i in range(0,k):
        bit = 2**(k-i-1)
        for j in range(0,2**k):
            b.add_edge((i,j),(i+1,j))
            b.add_edge((i,j),(i+1,j ^ bit))

    result = fnss.Topology(b)
    result.name = f'butterfly(k={k})'
    result.type = 'butterfly'
    for n in result.nodes:
        if isinstance(n, int):
            result.node[n]['type'] = 'host'
        else:
            result.node[n]['type'] = 'switch'

    return result

    

def hypercube_topology(k):
    intercon = fnss.topologies.topology.Topology(nx.hypercube_graph(k))
    intercon.name = 'hypergraph_topology(n=%d, size=%d)' % (k, 2**k)
    intercon.graph['type'] = 'hypergraph'
    for n in intercon.nodes:
        intercon.node[n]['type'] = 'host'
    return intercon

def dcell_topology(k, t):
    # t= number of hosts
    # k= level of the Dcell we need Dcell_k
    num_hosts_org = t
    level_Cell = k

    # return key for 0,0 & 0,1 values
    def get_key(dic_hosts, val, i):
        temp = -1
        for key, value in dic_hosts.items():
            if (val[0] == value[0] and val[1] == value[1]):
                temp += 1
                if (temp == i):
                    return key

    def get_key2(dic_hosts, i):

        for key, value in dic_hosts.items():
            start = dic_hosts[i][0][0]
            end = start + t_k[1] - 1
            return start, end

    #            return key

    def get_val(dic_hosts, key_inp):
        for key, value in dic_hosts.items():
            if (key_inp == key):
                return value
            # tup_flag =type(key) is tuple

    def get_val2(dic_hosts, key_inp):

        for key, value in dic_hosts.items():
            if (key_inp == key[0]):
                return value[0]
            elif (key_inp == key[1]):
                return value[1]

    def Build_DCell(k, t):
        for u in range(t):  # //k=4 - hosts
            v = topo.number_of_nodes()  # t index 5
            topo.add_node(v)  # server layer
            topo.node[v]['tier'] = 'server'  # tier has level of cell
            topo.node[v]['type'] = 'host'  # server/host layer
            topo.add_edge(i, v, type='host_edge')  # switch_/host_edge

        v = topo.number_of_nodes()
        return

    if t < 0 or k < 0:
        raise TypeError("Invalid arguments: args should be positive")

    topo = DatacenterTopology(type='dcell')
    topo.name = "dcell_topology(%d,%d)" % (k, t)

    ## Calculate params
    t_k = []
    g_k = []
    g_k.append(1)
    t_k.append(t)  # [0]=t
    loc_dic = np.zeros((t * (t + 1), 2))  # location strings
    print("value of loc_dic**********" + str(loc_dic))

    dic_hosts = {}
    level_hosts = {}

    result = 1
    if k > 0:
        for i in range(k):
            g_k.append(t_k[i] + 1)
            t_k.append(g_k[i + 1] * t_k[i])

    for item in g_k:
        result = result * item

    # create dic:
    dic_levels = 3  # (pow(2,k))+1
    temp_list = {}

    for i in range(dic_levels):
        temp = 'dic_' + str(i)
        temp_list[temp] = {}
        # temp={}

    # create mini switch; eg: num of hosts(t)=4 ==> num of dcell0 needed = n+1(5)
    print("Desired num of DCells: " + str(result))
    topo.add_nodes_from([v for v in range(result)],
                        layer='server', type='mini_switch')

    v = topo.number_of_nodes()
    nodes_post_mini_switches = v

    dic_0 = {}
    dic_1 = {}
    loc_test_dic = {}
    loc_test_dic_1d = {}
    idx = 0
    loc_idx = 0
    # created DCell0 Decimation
    for i in range(v, v + t_k[-1], t_k[0]):
        dic_0[idx] = [i, i + 1]

        loc_test_dic[i, i + 1] = [[loc_idx, 0], [loc_idx, 1]]

        loc_test_dic_1d[i] = [loc_idx, 0]
        loc_test_dic_1d[i + 1] = [loc_idx, 1]
        loc_idx += 1

        if (loc_idx == (g_k[1])):
            loc_idx = 0
        print(str(dic_0) + " elem val ")
        idx += 1

    # temp=get_val(loc_test_dic,22)#returns [0,0]; [0,1]
    print(loc_test_dic_1d)
    # created DCell1 Decimation - in terms of DCell0
    idx = 0
    for i in range(0, g_k[2] * t_k[1], g_k[1]):
        print('val of i ' + str(i))
        dic_1[idx] = [get_val(dic_0, i), get_val(dic_0, i + 1), get_val(dic_0, i + 2)]
        print(str(dic_1) + " elem val ")
        idx += 1
        if (idx == 7):
            break

    print(dic_0)
    # pdb.set_trace()
    last_elem_g_k = g_k[-1]
    # indx=0
    # idx_link=0
    if k > 0:
        for u in range(nodes_post_mini_switches):  # //k=4 - hosts

            for i in range(2):
                v = topo.number_of_nodes()  # t index 5
                topo.add_node(v)  # server layer
                topo.node[v]['tier'] = 'server'  # tier has level of cell
                topo.node[v]['type'] = 'host'  # server/host layer

                tmp = topo.number_of_nodes()  #
                print('Host edge between: ' + str(u) + '  ' + str(v))
                topo.add_edge(u, v, type='host_edge')  # switch_/host_edge
            # idx_link+=1

        v = topo.number_of_nodes()

    # connect the rest:
    # connecting individual dcells
    #####################################################
    idx_t = 0
    loc_var = []

    # location for smallest dcell to match
    for i in range(t + 1):
        for j in range(t):
            loc_var.append([i, j])
            # tmp=get_val(loc_test_dic,val1)

    # added links in between x DCell0s
    for i in range(7):  # DCell1
        for j in range(3):  # nums of dcell0s in dcell1
            v1 = dic_1[i][j]
            print(v1)

            for k in range(2):  # elems in dcell 0
                key1 = v1[k]
                elem1 = get_val2(loc_test_dic, key1)
                elem2 = [elem1[1] + 1, elem1[0]]

                temp_key2 = get_key(loc_test_dic_1d, elem2, i)  # need to obtain key

                start, end = get_key2(dic_1, i)
                print("     ")
                print("Val of i " + str(i) + " start val " + str(start) + " end val: " + str(end))
                print("     ")
                if temp_key2 >= start and temp_key2 <= end:
                    key2 = temp_key2

                if (elem1[0] < elem2[0]):
                    print("***********************************************")
                    print("val of i is " + str(k) + " elem1: " + str(elem1) + " elem2: " + str(elem2))
                    print("             ")
                    print("----------------------------------------------")
                    print("val of e1 " + str(key1) + " e2: " + str(key2))
                    print("----------------------------------------------")
                    print("             ")
                    topo.add_edge(key1, key2, type='link_edge')
                    print("***********************************************")

            print("out of k loop" + " val of j is: " + str(j))
        print("out of j loop")
    print("out of i loop")

    # print("value of loc of el1 "+str(elem1)+" el2 "+str(elem2))
    # v1=get_key(loc_var,get_key[i][i])
    # print
    # now add links in between x DCell1s

    print(loc_var)

    #####################################################
    # interconnecting dcell0s

    indx_1st_elms = [nodes_post_mini_switches]

    for i in range(7):
        if i == 0:
            tmp = nodes_post_mini_switches
        tmp += t_k[1]  # //num of dcell elemts 6
        indx_1st_elms.append(tmp)

    # interconnecting dcell1s

    elem1 = nodes_post_mini_switches
    for i in range(g_k[-1] - 1):
        print("start")
        elem2 = indx_1st_elms[i + 1]
        topo.add_edge(elem1, elem1, type='link_lev_1_edge')
        elem1 += 1

        print("val of elem1 is: " + str(elem1))

    return topo

def collapsed_core(num_aggr, num_access, num_hosts):
    if not all(isinstance(n, int) for n in (num_aggr, num_hosts, num_access)):
        raise TypeError("Invalid arguments: args should be integers")
    if num_aggr < 1 or num_hosts < 1 or num_access < 1:
        raise TypeError("Invalid arguments: args should be positive")

    topo = DatacenterTopology(nx.complete_bipartite_graph(num_aggr, num_access))
    topo.name = "collapsed_core_topology(%d,%d,%d)" % (num_aggr, num_access, num_hosts)
    topo.graph['type'] = 'collapsed_core'
    for u in range(num_aggr):
        topo.node[u]['tier'] = 'aggregation'
        topo.node[u]['type'] = 'switch'
        for v in topo.adj[u]:
            topo.adj[u][v]['type'] = 'aggregation_edge'
    for u in range(num_aggr, num_aggr + num_access):
        topo.node[u]['tier'] = 'access'
        topo.node[u]['type'] = 'switch'
        for _ in range(num_hosts):
            v = topo.number_of_nodes()
            topo.add_node(v)
            topo.node[v]['tier'] = 'leaf'
            topo.node[v]['type'] = 'host'
            topo.add_edge(u, v, type='edge_leaf')

    for x in range(num_aggr - 1):
        for y in range(x+1, num_aggr):
            topo.add_edge(x, y, type='cc_edges')

    return topo



def leaf_spine(num_leaf, num_spine, num_hosts):
    if not all(isinstance(x, int) for x in (num_leaf, num_spine, num_hosts)):
        raise TypeError("All args must be integers ")
    if num_spine < 1 or num_spine < 1 or num_hosts < 1:
        raise TypeError("All values must be positive")

    myTopo = DatacenterTopology(nx.complete_bipartite_graph(num_leaf, num_spine))
    myTopo.name = "leaf_spine_topology(%d,%d,%d)" % (num_leaf, num_spine, num_hosts)

    myTopo.graph['type'] = 'leaf_spine'

    for m in range(num_leaf):
        myTopo.node[m]['tier'] = 'leaf'
        myTopo.node[m]['type'] = 'switch'
        for v in myTopo.adj[m]:
            myTopo.adj[m][v]['type'] = 'leaf_edge'
    for n in range(num_leaf,num_leaf+num_spine):
        myTopo.node[n]['tier'] = 'spine'
        myTopo.node[m]['type'] = 'switch'
        for p in myTopo.adj[n]:
            if myTopo.adj[m][p]['type'] != 'leaf_edge':
                myTopo.adj[m][p]['type'] = 'spine_edge'
            else:
                myTopo.adj[m][p]['type'] = 'connection_edge'
            for _p_ in range(num_hosts):
                num_of_nodes = myTopo.number_of_hosts()
                myTopo.add_node(_p_)
                myTopo.node[num_of_nodes]['tier'] = 'host_edge'
                myTopo.node[num_of_nodes]['type'] = 'host'
                myTopo.add_edge(p,_p_, type='host_edge')

    return myTopo
