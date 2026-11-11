#include <vp/vp.hpp>  // -> menyediakan fasilitas dasar komponen GVSoC
#include <vp/itf/io.hpp>  // -> menyediakan interface IO, request, dan status transaksi

#include <iostream> //-> untuk menampilkan pesan melalui stf::count
#include <cstring> //-> untuk menyalin data std::count
#include <cstdint>//-> untuk tipe integer, unt32_t


// Deklarasi class DummyIP //
class DummyIp : public vp::Component
{
public:

    DummyIp(vp::ComponentConf &config);

    void reset(bool active); //-> mendeklarasikan metode reset milik DummyIp

private:
 // kenapa menggunakan static = membutukna handler untuk menerima pointer ke blok komponen dan pointer 
    static vp::IoReqStatus handle_req(  
        vp::Block *__this,
        vp::IoReq *req
    );

    vp::IoSlave input_itf; // -> mendeklarasikan interface slave milik dummyip

    uint32_t reg_value; // -> mendeklarasikan register virtual 32-bit yang digunakan untuk menyimpan data
};

  // contractor DummyIP (contractor adalah fungsi yang dijalankan ketika objek dummy ip dibuat)
DummyIp::DummyIp(vp::ComponentConf &config)
    : vp::Component(config)
{
    std::cout
        << "[DummyIp] C++ Module Loaded!"
        << std::endl;

// menghubungkan handler dengan interface //
    this->input_itf.set_req_meth(  // input_itf -> interface milik dummyip
        &DummyIp::handle_req
    );
 // port input // -> harus sesuai dengan namam pada python
    this->new_slave_port(
        "input",
        &this->input_itf
    );

    this->reg_value = 0;
}

 // fungsi reset dummyip //
void DummyIp::reset(bool active)
{
    if (active) // jika active true = maka cetak activate kalau false = cetak deactivated //
    {
        this->reg_value = 0;

        std::cout
            << "[DummyIp] System Reset Activated!"
            << std::endl;
   }
    else
    {
        std::cout
            << "[DummyIp] System Reset Deactivated!"
            << std::endl;
    }
}

// handler yang menangani request WRITE dan READ dari TestMaster //
vp::IoReqStatus DummyIp::handle_req(
    vp::Block *__this,
    vp::IoReq *req  // req -> merupakan objek yang membawa informasi transaksi dari TestMaster ke DummyIP 
)
{

  // mengubah pointer dengan static_cast tujuannya agar dapat mengakses anggota khusu dummyIP //
    DummyIp *_this =
        static_cast<DummyIp *>(__this);

// mengambil informasi dari request //
    uint64_t addr =  // alat request || mengetahui register yang diakses //
        req->get_addr(); // mengambil alamat yang diminta testmaster

    uint64_t size =  // ukuran requestt dalam byte || mengetahui jumlah byte data //
        req->get_size(); //mengambil ukuran transaksi 

    uint8_t *data =  // pointer ke buffer data || mengakses data yang ditulis atau ditempat hasil READ //
        req->get_data(); // mengambil pointer ke buffer data transaksi //


// validari alamat dan ukuran  alamat 0x0 dan ukuran transaksi 4 byte (||) artinya OR jika salah satu benar//
    if (addr != 0x0 ||
        size != sizeof(uint32_t))
    {
        return vp::IO_REQ_INVALID;
    }

// menentukan WRITE atau read //

    if (req->get_is_write()) // mengetahui request yang diterima operasi apa | true = write - false = READ
    {
    
    /// untuk proses WRITE ///
        std::memcpy(
            &_this->reg_value,
            data,
            sizeof(uint32_t)
        );

        std::cout
            << "[DummyIp WRITE] value = 0x"
            << std::hex
            << _this->reg_value
            << std::dec
            << std::endl;
    }
    else
    {
    
    // untuk proses READ //
        std::memcpy(
            data,
            &_this->reg_value,
            sizeof(uint32_t)
        );

        std::cout
            << "[DummyIp READ ] value = 0x"
            << std::hex
            << _this->reg_value
            << std::dec
            << std::endl;
    }


    return vp::IO_REQ_OK;
}

 // fungsi ini digunakan sebagai faktor untuk membuat objek dummmyip ketika model dimuat melalui mekanisme GVSoC yang menggunakan simbil gv_new
extern "C"  // "C" menggunkan C Language linkage untuk nama fungsi
vp::Component *gv_new(
    vp::ComponentConf &config
)
{
    return new DummyIp(config);
}


//extern "C"
//vp::Component *vp_constructor(
//    vp::ComponentConf &config
//)
//{
//    return new DummyIp(config);
//}
