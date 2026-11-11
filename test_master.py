import os
import gvsoc.systree as st

## membuat class TestMaster sebagai componen GVSOC #
class TestMaster(st.Component):

    # fungsi yang dijalankan ketika komponen testmaster pada gvsoc
    def __init__(self, parent, name):
        
        #memanggil constructor induk dari gvsoc
        super().__init__(parent, name)
        
        # Mendaftarkan dile test_master.cpp sebagai code c++ komponen testmaster
        self.add_sources([os.path.join(os.path.dirname(__file__), "test_master.cpp")])

#        self.add_sources([
#            os.path.join(
#                os.path.dirname(__file__),
#                'test_master.cpp'
#            )
#        ])

    # membuat fungsi untuk menghubungkan outpu testmaster ke interface tujuan
    def o_OUT(self, itf):
    
        #mendaftarkan koneksi port output testmaster ke interface dummyip
        self.itf_bind(        
            'out',            # nama port output yang didaftarkan pada testmaster.cpp
            itf,              # interface tujuan yang berasal dari dummy.i_INPUT()
            signature='oke'
        )
