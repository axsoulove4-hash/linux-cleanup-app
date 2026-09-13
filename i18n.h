#pragma once
#include <QtWidgets>
inline bool english=false;
inline QString ui(QString s){
    if(!english)return s;
    static const QVector<QPair<QString,QString>> words=[]{
        QVector<QPair<QString,QString>> v{
            {"Cochez les fichiers à supprimer définitivement. Fermez les applications concernées. Les fichiers modifiés depuis analyse seront ignorés.","Check the files to delete permanently. Close the affected apps. Files changed since the scan will be skipped."},
            {"fichiers seront supprimés sans passer par corbeille. Continuer ?","files will be permanently deleted without going to the trash. Continue?"},
            {"Suppression définitive des fichiers cochés, uniquement après confirmation.","Checked files are permanently deleted only after confirmation."},
            {"Double-clic catégorie : cochez les fichiers puis confirmez leur suppression.","Double-click a category: check files and confirm their deletion."},
            {"Supprimer après confirmation","Delete after confirmation"},
            {"Suppression définitive","Permanent deletion"},{"Supprimer","Delete"},{"Supprimés","Deleted"},{"Ignorés","Skipped"},{"Échecs","Failed"},{"fichiers","files"},
            {"Géré par Windows. Aucun nettoyage automatique.","Managed by Windows. No automatic cleanup."},
            {"Cache Internet Windows du compte.","Windows Internet cache for your account."},
            {"Ouvrir les applications Windows.","Open Windows app settings."},
            {"Journaux gérés par Windows.","Logs managed by Windows."},
            {"Miniatures gérées par Windows.","Thumbnails managed by Windows."},
            {"Ouvrir la corbeille Windows.","Open Windows Recycle Bin."},
            {"Fichiers temporaires anciens du compte.","Old temporary files for your account."},
            {"Tableau de bord","Dashboard"},{"Nettoyage","Cleanup"},{"Applications","Applications"},{"Stockage","Storage"},{"Éléments volumineux","Large files"},{"Paramètres","Settings"},
            {"Un système plus propre,\nune meilleure expérience","A cleaner system,\na better experience"},{"Linux propre\nDonnées locales","Clean Linux\nLocal data"},{"Bonjour !","Hello!"},
            {"Mesures locales · fichiers utilisateur accessibles","Local measurements · accessible user files"},{"Analyse locale en cours…","Scanning local files…"},{"Analyse en cours…","Scanning…"},{"Analyse au démarrage…","Starting scan…"},{"Analyse automatique désactivée","Startup scan disabled"},
            {"Fichiers candidats · tailles logiques","Cleanup candidates · file sizes"},{"Votre stockage","Your storage"},{"Volume utilisateur","Personal drive"},{"mesurés","measured"},{"utilisés","used"},{"libres","free"},
            {"Analyse en lecture seule","Read-only scan"},{"Vos fichiers restent sous votre contrôle.","Your files stay under your control."},{"Analyse rapide","Quick scan"},{"Sélectionnez ce que vous souhaitez nettoyer.","Choose what you want to clean."},
            {"Analyser à nouveau","Scan again"},{"Actualiser les données","Refresh"},{"Voir un aperçu des fichiers","Preview files"},{"Voir les détails ⌄","View details ⌄"},{"Voir détails","View details"},
            {"Caches utilisateur","App caches"},{"Caches anciens du compte utilisateur.","Old cache files for your account."},{"Caches anciens du compte.","Old cache files for your account."},
            {"Paquets inutilisés","Unused packages"},{"Fichiers de logs","Log files"},{"Fichiers .log anciens du compte.","Old .log files for your account."},{"Miniatures","Thumbnails"},{"Vignettes d’images et de vidéos.","Image and video previews."},
            {"Corbeille","Trash"},{"Fichiers présents dans la corbeille\nutilisateur.","Files already in your trash."},{"Fichiers temporaires","Temporary files"},{"Fichiers /tmp anciens appartenant\nà votre utilisateur.","Old /tmp files owned by you."},
            {"Suggestions DNF / APT / pacman.\nAucune suppression automatique.","DNF / APT / pacman suggestions.\nNo automatic removal."},{"Recréable","Rebuildable"},{"À vérifier","Review first"},{"Paquets","Packages"},{"Temporaires","Temporary"},{"Vérifier ","Review "},
            {"Confirmation obligatoire · caches vers corbeille · paquets / système : consultation.","Confirmation required · caches go to trash · system packages are read-only."},
            {"Analyse : ","Scanned: "},{" inaccessible(s)"," inaccessible"},{" fichiers mesurés"," files measured"},{" jours"," days"},{" Mo"," MB"},{" Go"," GB"},{" Ko"," KB"},{" To"," TB"},
            {"Mesures utilisateur · cache / miniatures anciens · paquets : suggestions seulement","Your files · old caches and thumbnails · package suggestions only"},
            {"Catégorie","Category"},{"Mesuré","Size"},{"Action disponible","Available action"},{"Emplacement","Location"},{"Vers corbeille","Move to trash"},{"Consultation","Read only"},
            {"Aperçu complet / nettoyage des catégories cochées au tableau de bord","Preview and clean the categories selected on the dashboard"},
            {"Double-clic catégorie → liste précise. Vidage corbeille et suppressions système restent manuels.","Double-click a category to see its files. Emptying trash and removing system files remain manual."},
            {"Vos applications","Your applications"},{"Rechercher une application…","Search applications…"},{"Installée pour vous","Installed for you"},{"Installée sur cet ordinateur","Installed on this computer"},
            {"applications avec un raccourci. Les composants techniques ne sont pas affichés.","apps with a launcher. Technical components are hidden."},{"Informations sur l’application","Application details"},{"Nom","Name"},{"Installation","Installation"},
            {"Vos disques","Your drives"},{"Espace occupé et disponible. Les montages techniques sont masqués.","Used and available space. Technical mounts are hidden."},{"Disque principal","Main drive"},{"Disque supplémentaire","Additional drive"},
            {"disque(s) détecté(s)","drive(s) detected"},{"Aucun disque accessible détecté.","No accessible drives found."},{"disponibles sur","free of"},{"Occupé","Used"},{"Disponible","Available"},{"Ouvrir ce disque","Open this drive"},
            {"Repérez ce qui prend de la place","See what takes up space"},{"Rechercher un fichier…","Search files…"},{"Tous les fichiers","All files"},{"Images","Images"},{"Vidéos","Videos"},{"Documents","Documents"},{"Autres","Other"},
            {"Fichier","File"},{"Dossier","Folder"},{"Taille","Size"},{"Modifié","Modified"},{"Ouvrir le dossier","Open folder"},{"Sélectionnez un fichier pour voir ses détails.","Select a file to see its details."},
            {"Aucun fichier ne correspond aux filtres.","No files match these filters."},{"fichiers affichés","files shown"},{"Les plus gros en premier","Largest first"},
            {"Mode clair","Light mode"},{"Apparence","Appearance"},{"Choisir couleur","Choose color"},{"Accent","Accent"},{"Analyser au lancement","Scan at startup"},{"Démarrage","Startup"},
            {"Âge minimum caches, logs, /tmp","Minimum cache, log and temp file age"},{"Seuil fichiers volumineux","Large file threshold"},{"Dossier fichiers volumineux","Folder to scan for large files"},
            {"Un chemin absolu par ligne","One absolute path per line"},{"Exclusions de toutes les analyses","Folders excluded from every scan"},{"Enregistrer et réanalyser","Save and rescan"},{"Langue","Language"},
            {"Confirmation toujours obligatoire. Aucun démarrage root ni nettoyage automatique.","Confirmation is always required. No automatic cleanup or root access."},
            {"Indique dossier absolu existant.","Choose an existing folder with an absolute path."},{"Exclusions","Exclusions"},{"Utilise chemins absolus.","Use absolute paths."},
            {"Fichiers mesurés — aperçu","Scanned files — preview"},{"Fermer","Close"},{"Aperçu de la sélection","Selected files"},{"Analyse en cours. Attends résultats.","Scan in progress. Please wait for results."},
            {"Taille logique des fichiers. Ferme les applications concernées avant nettoyage.\nCache et miniatures : déplacement vers corbeille. Autres catégories : consultation uniquement.","File sizes. Close the affected apps before cleaning.\nCaches and thumbnails move to trash. Other categories are read-only."},
            {"Déplacer caches / miniatures sélectionnés vers corbeille","Move selected caches and thumbnails to trash"},{"Confirmer déplacement","Confirm move"},
            {"Déplacer uniquement caches et miniatures de cette liste vers corbeille ?\nRécupérables depuis gestionnaire de fichiers. Espace disque pas libéré avant vidage manuel.","Move only the caches and thumbnails in this list to trash?\nYou can restore them in your file manager. Space is not freed until you empty the trash."},
            {"Résultat","Result"},{" fichiers déplacés, "," files moved, "},{" ignorés ou en échec.\nRestauration possible depuis corbeille."," skipped or failed.\nYou can restore moved files from the trash."},
            {"Aucun orphelin DNF","No unused DNF packages"},{"Aucun orphelin APT","No unused APT packages"},{"Non pris en charge","Not supported"},{"Indisponible","Unavailable"},{"Non interrogé","Not queried"}
        };
        std::sort(v.begin(),v.end(),[](const auto&a,const auto&b){return a.first.size()>b.first.size();});return v;
    }();
    // Replace only source text, once per match; never retranslate generated output.
    QString out;for(int i=0;i<s.size();){bool hit=false;for(const auto &w:words)if(QStringView(s).mid(i).startsWith(w.first)){out+=w.second;i+=w.first.size();hit=true;break;}if(!hit)out+=s[i++];}
    return out;
}
inline void localizeWidgets(QWidget *root){
    QList<QWidget*> widgets=root->findChildren<QWidget*>();widgets.prepend(root);
    for(auto *w:widgets){
        if(auto *label=qobject_cast<QLabel*>(w))label->setText(ui(label->text()));
        if(auto *button=qobject_cast<QAbstractButton*>(w))button->setText(ui(button->text()));
        if(auto *edit=qobject_cast<QLineEdit*>(w))edit->setPlaceholderText(ui(edit->placeholderText()));
        if(auto *edit=qobject_cast<QPlainTextEdit*>(w))edit->setPlaceholderText(ui(edit->placeholderText()));
        if(auto *t=qobject_cast<QTableWidget*>(w))for(int c=0;c<t->columnCount();c++)if(t->horizontalHeaderItem(c))t->horizontalHeaderItem(c)->setText(ui(t->horizontalHeaderItem(c)->text()));
    }
}
class LanguageDialogs:public QObject{
public:
    using QObject::QObject;
    bool eventFilter(QObject *o,QEvent *e) override{
        if(e->type()==QEvent::Show)if(auto *box=qobject_cast<QMessageBox*>(o)){
            box->setWindowTitle(ui(box->windowTitle()));box->setText(ui(box->text()));box->setInformativeText(ui(box->informativeText()));
            for(auto *b:box->buttons()){
                auto id=box->standardButton(b);
                if(id==QMessageBox::Yes)b->setText(english?"Yes":"Oui");
                if(id==QMessageBox::No)b->setText(english?"No":"Non");
                if(id==QMessageBox::Cancel)b->setText(english?"Cancel":"Annuler");
            }
        }
        return QObject::eventFilter(o,e);
    }
};
