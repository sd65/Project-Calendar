#include <QMessageBox>
#include <QRadioButton>

#include "core/headers/projet.h"

#include "ui/headers/ajouterTacheUnitaire.h"
#include "ui_ajouterTacheUnitaire.h"
#include "ui/headers/uiHelpers.h"

ajouterTacheUnitaire::ajouterTacheUnitaire(QWidget *parent, Projet* pprojet) :
    QDialog(parent),
    ui(new Ui::ajouterTacheUnitaire)
{
    ui->setupUi(this);
    projet = pprojet;
    ui->disponibilite->setDate(QDate::currentDate());
    ui->disponibilite->setMinimumDate(projet->getDisponibilite());
    ui->disponibilite->setMaximumDate(projet->getEcheance());

    ui->echeance->setDate(QDate::currentDate().addMonths(1));
    ui->echeance->setMinimumDate(projet->getDisponibilite());
    ui->echeance->setMaximumDate(projet->getEcheance());

    ui->boxconst->setDisabled(true);

    QListWidgetItem *it;
    foreach(Tache* tache, *projet->getTaches())
    {
        it= new QListWidgetItem(tache->getNom());
        ui->listeTachesPred->addItem(it);
    }
}

ajouterTacheUnitaire::~ajouterTacheUnitaire()
{
    delete ui;
}

/*!
  Override default behaviour of accept buttons or enter key. Performs checks before insert datas.
 */
void ajouterTacheUnitaire::accept() {
    bool statusOk = true;
    QString message;

    if(ui->nom->text().isEmpty())
    {
        message += "Remplissez le titre du de la tache !";
        statusOk = false;
    }

    if(ui->duree->time() < QTime(0,1))
    {
        message += "\nLa durée est invalide !";
        statusOk = false;
    }

    if(ui->duree->time().hour() > 12)
    {
        message += "\nLa durée ne peut dépasser 12 heures !";
        statusOk = false;
    }

    if(ui->disponibilite->date() > ui->echeance->date())
    {
        message += "\nL'échéance est antérieure à la date de disponibilité !";
        statusOk = false;
    }

    if(ui->constraitsToogle->isChecked() && ui->echeance->date() < projet->getTaches()->value(ui->listeTachesPred->currentItem()->text())->getEcheance())
    {
        message += "\nL'échéance de votre tache précédente est supérieure à la date d'échance de cette tache !";
        statusOk = false;
    }

    if(statusOk)
    {
        Tache* pred = nullptr;
        if(ui->constraitsToogle->isChecked())
            pred = projet->getTaches()->value(ui->listeTachesPred->currentItem()->text());
        projet->creerTacheUnitaire(ui->nom->text(), ui->duree->time(), ui->preemptive->isChecked(), pred, ui->disponibilite->date(), ui->echeance->date());
        this->close();
    }
    else
        if (showErrorsAndAskWhatToDo(message))
            this->close();
}
