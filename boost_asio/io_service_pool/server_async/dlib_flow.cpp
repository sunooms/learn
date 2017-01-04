
#include "dlib_flow.h"
#include "hy_dynamic_dmp.pb.h"

bool dlib_flow::dynamic_process(package_message& res_msg)
{
    std::string res;
    proto_parse(res);

    if(res.length() == 0){
        return false;
    }

    HyDynamicDmpHeader reqhead;
    reqhead.wLength = res.length() + sizeof(reqhead);
    HyDynamicDmpHeader::host2net(&reqhead);

    memcpy(res_msg.data(), (const char *)&reqhead, sizeof(reqhead));
    memcpy(res_msg.body(), (const char *)res.c_str(), res.length());
    res_msg.body_length(res.length());

    return true;
}

int dlib_flow::proto_parse(std::string& retmsg)
{
    using namespace hy_dynamic_dmp;

    hyDMPRequest  req;
    hyDMPResponse res;
    req.ParseFromArray(read_msg_.body(), read_msg_.body_length());
    printf("now req id:%s\n", req.id().c_str());
    printf("now req media:%s\n", req.media().c_str());

    res.set_id(req.id());

    for(int i=0; i<req.ad_size(); i++)
    {
        const Ad &ad = req.ad(i);

        for(int j=0; j<ad.mat_size(); j++)
        {
            const Mat &mat = ad.mat(j);

            MatAd *tmp_matad = res.add_mtad();
            tmp_matad->set_matid(mat.id());
            tmp_matad->set_adid(ad.id());
            tmp_matad->set_adspaceid(ad.adspaceid());

        }

    }

    res.SerializeToString(&retmsg);
    return 0;
}
