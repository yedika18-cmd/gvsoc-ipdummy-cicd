import gvsoc.systree as st
import gvsoc.runner
import vp.clock_domain

from dummy_ip import DummyIp
from test_master import TestMaster


GAPY_TARGET = True


class Top(st.Component):

    def __init__(
        self,
        parent,
        name,
        parser,
        options
    ):

        super().__init__(
            parent,
            name,
            options=options
        )


        clock = vp.clock_domain.Clock_domain(
            self,
            'clock',
            frequency=100000000
        )


        dummy = DummyIp(
            self,
            'dummy'
        )


        master = TestMaster(
            self,
            'master'
        )


        master.o_OUT(
            dummy.i_INPUT()
        )


        clock.o_CLOCK(
            dummy.i_CLOCK()
        )


        clock.o_CLOCK(
            master.i_CLOCK()
        )


class Target(gvsoc.runner.Target):

    def __init__(
        self,
        parser,
        options
    ):

        super().__init__(
            parser,
            options,
            model=Top,
            description=
            'Dummy IP Echo Test'
        )
