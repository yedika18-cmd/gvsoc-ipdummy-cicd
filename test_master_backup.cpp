#include <vp/vp.hpp> // -> dibutuhkan untuk menggunakan class dasar komponen GVSoC 
#include <vp/itf/io.hpp>  // -> karena testmaster menggunakan vp::iomaster dan objek vp:ioreq

#include <iostream> // -> digunakan untuk mencetak pesan seperti Starting Echo Test, TX, RX
#include <cstdint> // -> digunakan untuk menyediakan tipe data seperti uint32_t


 // Deklarasi class Testmaster ///

class TestMaster : public vp::Component
{
public:

    TestMaster(vp::ComponentConf &config);

    void reset(bool active);

private:

    vp::IoMaster out_itf;

};

   /// constructor TestMaster ///
TestMaster::TestMaster(
    vp::ComponentConf &config
)
    : vp::Component(config)
{
    this->new_master_port(
        "out",
        &this->out_itf
    );
}

 // FUNGSI RESET //
void TestMaster::reset(bool active)
{
    if (active) // -> nilai (active): true = fungsi langsung berhenti false = program melanjutkan instrusi
    {
        return;
    }


    std::cout
        << "[TestMaster] Starting Echo Test..."
        << std::endl;

 /// MENENTUKAN NILAI TX DAN RX ///
    uint32_t tx_data =
        0x182005;

    uint32_t rx_data =
        0;

/// MEMBUAT TRANSAKSI WRITE ///
    vp::IoReq write_req;

    write_req.init();

    write_req.set_addr(0x0);

    write_req.set_size(
        sizeof(tx_data)
    );

    write_req.set_data(
        reinterpret_cast<uint8_t *>(
            &tx_data
        )
    );

    write_req.set_is_write(true);  /// --> PROSES WRITE

  // MENGIRIM WRITE KE DUMMY IP //
    vp::IoReqStatus write_status =
        this->out_itf.req(
            &write_req
        );

   //// MEMERIKSA STATUS WRITE ///
    if (write_status != vp::IO_REQ_OK) // --> jika proses write bukan IO_REQ_OK (GAGAL) maka proses WRITE GAGAL
    { 
        std::cout
            << "[ERROR] WRITE failed"
            << std::endl;

        return;
    }

   // MEMBUAT TRANSAKSI READ //
    vp::IoReq read_req;

    read_req.init();

    read_req.set_addr(0x0);

    read_req.set_size(
        sizeof(rx_data)
    );

    read_req.set_data(
        reinterpret_cast<uint8_t *>(
            &rx_data
        )
    );

    read_req.set_is_write(false);  /// --> PROSES READ

   ///// PROSES MENGIRIM READ DAN MEMERIKSA STATUS APAKAH READ BERHASIL ATAU TIDAK ////
    vp::IoReqStatus read_status =
        this->out_itf.req(
            &read_req
        );


    if (read_status != vp::IO_REQ_OK)
    {
        std::cout
            << "[ERROR] READ failed"
            << std::endl;

        return;
    }

//// Menampilkan TX DAN TX /////
    std::cout
        << "[TestMaster] TX = 0x"    
        << std::hex
        << tx_data
        << std::endl;

    std::cout
        << "[TestMaster] RX = 0x"
        << rx_data
        << std::dec
        << std::endl;


//// MEMBANDINGKAN NILAI TX DAN RX /////
    if (tx_data == rx_data)   /// -> proses melakukan perbandingan anara tx dan rx
    {
        std::cout
            << "================================="
            << std::endl;

        std::cout
            << "[PASS] Dummy IP Echo/Loopback OK!"
            << std::endl;

        std::cout
            << "================================="
            << std::endl;
    }
    else
    {
        std::cout
            << "[FAIL] Dummy IP Echo/Loopback"
            << std::endl;
    }
}


extern "C"
vp::Component *gv_new(
    vp::ComponentConf &config
)
{
    return new TestMaster(config);
}


//extern "C"
//vp::Component *vp_constructor(
//    vp::ComponentConf &config
//)
//{
//    return new TestMaster(config);
//}
