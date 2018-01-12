import paho.mqtt.publish as publish
import telebot
bot = telebot.TeleBot("token")
@bot.message_handler(commands=['start', 'help'])
def send_welcome(message):
        bot.reply_to(message, "Holi, bienvenido al chatbot mas kulz <3")
        bot.reply_to(message, "Comandos: \n EncenderSalida1, EncenderSalida2, EncenderSalida3, EncenderSalida4, EncenderSalida4, EncenderTodo.\n ApagarSalida1, ApagarSalida2, ApagarSalida3, ApagarSalida4, ApagarTodo\n Gracias")
@bot.message_handler(func=lambda message: True)
def echo_all(message):
        bot.reply_to(message, message.text)
        if(message.text == 'EncenderSalida1'):
                publish.single('/salida1','1',hostname='localhost')
                print("Salida 1 encendida.")
                bot.reply_to(message, "Salida 1 encendida.")
        elif(message.text == 'ApagarSalida1'):
                publish.single('/salida1','0',hostname='localhost')
                print("Salida 1 apagada")
                bot.reply_to(message, "Salida 1 apagada.")
        elif(message.text == 'EncenderSalida2'):
                publish.single('/salida2','1',hostname='localhost')
                print("Salida 2 encendida.")
                bot.reply_to(message, "Salida 2 encendida.")
        elif(message.text == 'ApagarSalida2'):
                publish.single('/salida2','0',hostname='localhost')
                print("Salida 2 apagada.")
                bot.reply_to(message, "Salida 2 apagada.")
        elif(message.text == 'EncenderSalida3'):
                publish.single('/salida3','1',hostname='localhost')
                print("Salida 3 encendida.")
                bot.reply_to(message, "Salida 3 encendida.")
        elif(message.text == 'ApagarSalida3'):
                publish.single('/salida3','0',hostname='localhost')
                print("Salida 3 apagada.") 
                bot.reply_to(message, "Salida 3 apagada.")
        elif(message.text == 'EncenderSalida4'):
                publish.single('/salida4','1',hostname='localhost')
                print("Salida 4 encendida.")
                bot.reply_to(message, "Salida 4 encendida.")
        elif(message.text == 'ApagarSalida4'):
                publish.single('/salida4','0',hostname='localhost')
                print("Salida 4 apagada.") 
                bot.reply_to(message, "Salida 4 apagada.")
        elif(message.text == 'ApagarTodo'):
                publish.single('/salida4','0',hostname='localhost')
                publish.single('/salida1','0',hostname='localhost')
                publish.single('/salida2','0',hostname='localhost')
                publish.single('/salida3','0',hostname='localhost')
                bot.reply_to(message, "Todo encendido.")
                print("apagando todo.") 
        elif(message.text == 'EncenderTodo'):
                publish.single('/salida4','1',hostname='localhost')
                publish.single('/salida1','1',hostname='localhost')
                publish.single('/salida2','1',hostname='localhost')
                publish.single('/salida3','1',hostname='localhost')
                print("apagando todo.") 
                bot.reply_to(message, "Todo apagado")
        #print(message)
        print(message.text)
print("iniciando programa")
bot.polling(none_stop=False,timeout=30)
