#!/usr/bin/env python2

import Tkinter as tk
import json
import io
from controller import *

with io.open('./src/gui_cv_algorithms/gui.json', 'r', encoding='utf-8') as f:
    gui = json.load(f)


def set_text(entry, text):
    entry.delete(0, tk.END)
    entry.insert(0, text)
    return


def create_control_panel(parent, controller, save_command):
    panel = tk.Label(parent)

    button1 = tk.Button(panel, text=gui['backButtonName'], command=lambda: controller.show_frame(HomePage))
    button1.pack(side='left')

    button2 = tk.Button(panel, text=gui['saveButtonName'],command=save_command)
    button2.pack(side='right')

    panel.pack(fill='both')


class CalibrationApp(tk.Tk):

    def __init__(self, *args, **kwargs):
        tk.Tk.__init__(self, *args, **kwargs)

        self.title(gui['appTitle'])
        container = tk.Frame(self)

        container.pack(side='top', expand=True)

        container.grid_rowconfigure(0, weight=1)
        container.grid_columnconfigure(0, weight=1)

        self.frames = {}

        for F in (HomePage, QrCodePage, LaneRecognitionPage):
            frame = F(container, self)

            self.frames[F] = frame

            frame.grid(row=0, column=0, sticky='nsew')

        self.show_frame(HomePage)

    def show_frame(self, cont):
        frame = self.frames[cont]
        frame.tkraise()


class HomePage(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)
        label = tk.Label(self, text=gui['HomePage']['title'], font=gui['font'])
        label.pack(pady=10, padx=10)

        button = tk.Button(self, text=gui['QrCodePage']['title'], command=lambda: controller.show_frame(QrCodePage))
        button.pack()

        button2 = tk.Button(self, text=gui['LaneRecognitionPage']['title'],
                            command=lambda: controller.show_frame(LaneRecognitionPage))
        button2.pack()


class QrCodePage(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)

        qr_code_page_json = gui['QrCodePage']

        label = tk.Label(self, text=qr_code_page_json['title'], font=gui['font'])
        label.pack(pady=10, padx=10)

        # headers start
        table_json = qr_code_page_json['table']

        table = tk.Label(self)

        frame0 = tk.Frame(table)
        frame0.grid(row=0, column=0)
        cell0 = tk.Label(frame0, text='', width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        cell0.pack()

        frame1 = tk.Frame(table)
        frame1.grid(row=0, column=1)
        cell1 = tk.Label(frame1, text=table_json['kernel'], width=table_json['cellSize'][0],
                         height=table_json['cellSize'][1])
        cell1.pack()

        frame2 = tk.Frame(table)
        frame2.grid(row=0, column=2)
        cell2 = tk.Label(frame2, text=table_json['times'], width=table_json['cellSize'][0],
                         height=table_json['cellSize'][1])
        cell2.pack()

        frame3 = tk.Frame(table)
        frame3.grid(row=0, column=3)
        cell3 = tk.Label(frame3, text=table_json['kernelDivider'], width=table_json['cellSize'][0],
                         height=table_json['cellSize'][1])
        cell3.pack()

        frame4 = tk.Frame(table)
        frame4.grid(row=1, column=0)
        cell4 = tk.Label(frame4, text=table_json['sharp'], width=table_json['cellSize'][0],
                         height=table_json['cellSize'][1])
        cell4.pack()

        frame5 = tk.Frame(table)
        frame5.grid(row=2, column=0)
        cell5 = tk.Label(frame5, text=table_json['erode'], width=table_json['cellSize'][0],
                         height=table_json['cellSize'][1])
        cell5.pack()
        # header end

        # data start
        self.data = load_data('QrCode')

        frame6 = tk.Frame(table)
        frame6.grid(row=1, column=1)
        cell6 = tk.Label(frame6, width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        entry6 = tk.Entry(cell6)
        set_text(entry6, self.data['sharp']['kernel'])
        entry6.pack()
        cell6.pack()

        frame7 = tk.Frame(table)
        frame7.grid(row=1, column=2)
        cell7 = tk.Label(frame7, width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        entry7 = tk.Entry(cell7)
        set_text(entry7, self.data['sharp']['times'])
        entry7.pack()
        cell7.pack()

        frame8 = tk.Frame(table)
        frame8.grid(row=1, column=3)
        cell8= tk.Label(frame8, width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        entry8 = tk.Entry(cell8)
        set_text(entry8, self.data['sharp']['kernelDivider'])
        entry8.pack()
        cell8.pack()

        frame9 = tk.Frame(table)
        frame9.grid(row=2, column=1)
        cell9 = tk.Label(frame9, width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        entry9 = tk.Entry(cell9)
        set_text(entry9, self.data['erode']['kernel'])
        entry9.pack()
        cell9.pack()

        frame10 = tk.Frame(table)
        frame10.grid(row=2, column=2)
        cell10 = tk.Label(frame10, width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        entry10 = tk.Entry(cell10)
        set_text(entry10, self.data['erode']['times'])
        entry10.pack()
        cell10.pack()

        frame11 = tk.Frame(table)
        frame11.grid(row=2, column=3)
        cell11 = tk.Label(frame11, width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        entry11 = tk.Entry(cell11)
        set_text(entry11, self.data['erode']['kernelDivider'])
        entry11.pack()
        cell11.pack()
        # data end

        table.pack()

        create_control_panel(self, controller, save_command=lambda: save_data('QrCode', data={
                            'sharp': {
                                'kernel': entry6.get(),
                                'times': entry7.get(),
                                'kernelDivider': entry8.get()
                            },
                            'erode': {
                                'kernel': entry9.get(),
                                'times': entry10.get(),
                                'kernelDivider': entry11.get()
                            }
                        }))


class LaneRecognitionPage(tk.Frame):

    def __init__(self, parent, controller):
        tk.Frame.__init__(self, parent)

        lane_recognition_page_json = gui['LaneRecognitionPage']

        table_json = lane_recognition_page_json['table']

        table = tk.Label(self)

        label = tk.Label(self, text=gui['LaneRecognitionPage']['title'], font=gui['font'])
        label.pack(pady=10, padx=10)

        # headers start
        frame0 = tk.Frame(table)
        frame0.grid(row=0, column=0)
        cell0 = tk.Label(frame0, text='', width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        cell0.pack()

        frame1 = tk.Frame(table)
        frame1.grid(row=0, column=1)
        cell1 = tk.Label(frame1, text=table_json['kernel'], width=table_json['cellSize'][0],
                         height=table_json['cellSize'][1])
        cell1.pack()

        frame2 = tk.Frame(table)
        frame2.grid(row=0, column=2)
        cell2 = tk.Label(frame2, text=table_json['times'], width=table_json['cellSize'][0],
                         height=table_json['cellSize'][1])
        cell2.pack()

        frame3 = tk.Frame(table)
        frame3.grid(row=0, column=3)
        cell3 = tk.Label(frame3, text=table_json['kernelDivider'], width=table_json['cellSize'][0],
                         height=table_json['cellSize'][1])
        cell3.pack()

        frame4 = tk.Frame(table)
        frame4.grid(row=1, column=0)
        cell4 = tk.Label(frame4, text=table_json['sharp'], width=table_json['cellSize'][0],
                         height=table_json['cellSize'][1])
        cell4.pack()
        # header end

        # data start
        self.data = load_data('LaneRecognition')

        frame5 = tk.Frame(table)
        frame5.grid(row=1, column=1)
        cell5 = tk.Label(frame5, width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        entry5 = tk.Entry(cell5)
        set_text(entry5, self.data['sharp']['kernel'])
        entry5.pack()
        cell5.pack()

        frame6 = tk.Frame(table)
        frame6.grid(row=1, column=2)
        cell6 = tk.Label(frame6, width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        entry6 = tk.Entry(cell6)
        set_text(entry6, self.data['sharp']['times'])
        entry6.pack()
        cell6.pack()

        frame7 = tk.Frame(table)
        frame7.grid(row=1, column=3)
        cell7 = tk.Label(frame7, width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        entry7 = tk.Entry(cell7)
        set_text(entry7, self.data['sharp']['kernelDivider'])
        entry7.pack()
        cell7.pack()
        # data end

        table.pack()

        table1 = tk.Label(self)

        #header start
        frame8 = tk.Frame(table1)

        frame8.grid(row=0, column=0)
        cell8 = tk.Label(frame8, text=table_json['laneColor'], width=table_json['cellSize'][0],
                         height=table_json['cellSize'][1])
        cell8.pack()

        frame9 = tk.Frame(table1)
        frame9.grid(row=1, column=0)
        cell9 = tk.Label(frame9, text=table_json['widthLineRange'], width=table_json['cellSize'][0],
                         height=table_json['cellSize'][1])
        cell9.pack()

        frame10 = tk.Frame(table1)
        frame10.grid(row=2, column=0)
        cell10 = tk.Label(frame10, text=table_json['laneWidth'], width=table_json['cellSize'][0],
                         height=table_json['cellSize'][1])
        cell10.pack()

        frame11 = tk.Frame(table1)
        frame11.grid(row=3, column=0)
        cell11 = tk.Label(frame11, text=table_json['laneWidthEps'], width=table_json['cellSize'][0],
                         height=table_json['cellSize'][1])
        cell11.pack()

        frame12 = tk.Frame(table1)
        frame12.grid(row=4, column=0)
        cell12 = tk.Label(frame12, text=table_json['widthLineRange'], width=table_json['cellSize'][0],
                         height=table_json['cellSize'][1])
        cell12.pack()

        frame13 = tk.Frame(table1)
        frame13.grid(row=5, column=0)
        cell13 = tk.Label(frame13, text=table_json['laneTreshold'], width=table_json['cellSize'][0],
                         height=table_json['cellSize'][1])
        cell13.pack()
        #header end

        # data start
        self.data = load_data('LaneRecognition')

        frame14 = tk.Frame(table1)
        frame14.grid(row=0, column=1)
        cell14 = tk.Label(frame14, width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        entry14 = tk.Entry(cell14)
        set_text(entry14, self.data['laneColor'])
        entry14.pack()
        cell14.pack()

        frame15 = tk.Frame(table1)
        frame15.grid(row=1, column=1)
        cell15 = tk.Label(frame15, width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        entry15 = tk.Entry(cell15)
        set_text(entry15, self.data['widthLineRange'])
        entry15.pack()
        cell15.pack()

        frame16 = tk.Frame(table1)
        frame16.grid(row=2, column=1)
        cell16 = tk.Label(frame16, width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        entry16 = tk.Entry(cell16)
        set_text(entry16, self.data['laneWidth'])
        entry16.pack()
        cell16.pack()

        frame17 = tk.Frame(table1)
        frame17.grid(row=3, column=1)
        cell17 = tk.Label(frame17, width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        entry17 = tk.Entry(cell17)
        set_text(entry17, self.data['laneWidthEps'])
        entry17.pack()
        cell17.pack()

        frame18 = tk.Frame(table1)
        frame18.grid(row=4, column=1)
        cell18 = tk.Label(frame18, width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        entry18 = tk.Entry(cell18)
        set_text(entry18, self.data['widthLineRange'])
        entry18.pack()
        cell18.pack()

        frame19 = tk.Frame(table1)
        frame19.grid(row=5, column=1)
        cell19 = tk.Label(frame19, width=table_json['cellSize'][0], height=table_json['cellSize'][1])
        entry19 = tk.Entry(cell19)
        set_text(entry19, self.data['laneTreshold'])
        entry19.pack()
        cell19.pack()
        # data end

        table1.pack()

        create_control_panel(self, controller, lambda: save_data('LaneRecognition', data={
            'laneWidth': entry15.get(),
            'widthLineRange': entry15.get(),
            'laneColor': entry14.get(),
            'sharp': {
                'kernel': entry5.get(),
                'times': entry6.get(),
                'kernelDivider': entry7.get()
            },
            'laneWidthEps': entry17.get(),
            'laneTreshold': entry18.get()
        }))
