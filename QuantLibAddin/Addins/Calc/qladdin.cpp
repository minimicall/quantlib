#include <string.h>
#include "qladdin.hpp"
#include <cppuhelper/factory.hxx>
#include <cppuhelper/implementationentry.hxx>
#include <osl/interlck.h>
#include <osl/mutex.hxx>
#include <rtl/uuid.h>

#define ADDIN_SERVICE	"com.sun.star.sheet.AddIn"
#define MY_SERVICE		"com.sun.star.sheet.addin.QL"
#define MY_IMPLNAME		"com.sun.star.sheet.addin.QLImpl"

using namespace ::rtl;
using namespace ::com::sun::star;

REF( uno::XInterface ) SAL_CALL QL_CreateInstance(
    REF( uno::XComponentContext ) const & xContext ) SAL_THROW( () ) {
    return static_cast< lang::XTypeProvider * >( new QLAddin() );
}
static struct ::cppu::ImplementationEntry s_component_entries [] = {
    {
        QL_CreateInstance, QLAddin::getImplementationName_Static,
        QLAddin::getSupportedServiceNames_Static,
		::cppu::createSingleComponentFactory, 0, 0
    },
    { 0, 0, 0, 0, 0, 0 }
};
extern "C" {
void SAL_CALL component_getImplementationEnvironment( const sal_Char** ppEnvTypeName, uno_Environment** ppEnv ) {
	*ppEnvTypeName = CPPU_CURRENT_LANGUAGE_BINDING_NAME;
}
sal_Bool SAL_CALL component_writeInfo( lang::XMultiServiceFactory * xMgr, registry::XRegistryKey * xRegistry ) {
    return ::cppu::component_writeInfoHelper(
        xMgr, xRegistry, s_component_entries );
}
void * SAL_CALL component_getFactory(
    sal_Char const * implName, lang::XMultiServiceFactory * xMgr,
    registry::XRegistryKey * xRegistry )
{
    return ::cppu::component_getFactoryHelper(
        implName, xMgr, xRegistry, s_component_entries );
}
} // extern "C"

// XInterface
CSS::uno::Any QLAddin::queryInterface( CSS::uno::Type const & type ) THROWDEF_RTE
{
    if (type.equals( ::getCppuType( (REF ( XInterface ) const *)0 ) )) {
        // return XInterface interface (resolve ambiguity by casting to lang::XTypeProvider)
        REF ( XInterface ) x( static_cast< CSS::lang::XTypeProvider * >( this ) );
        return makeAny( x );
    }
    if (type.equals( ::getCppuType( (REF ( CSS::lang::XTypeProvider ) const *)0 ) )) {
        // return XInterface interface
        REF ( XInterface ) x( static_cast< CSS::lang::XTypeProvider * >( this ) );
        return makeAny( x );
    }
    if (type.equals( ::getCppuType( (REF ( CSS::lang::XServiceName ) const *)0 ) )) {
        // return XServiceName interface
        REF ( CSS::lang::XServiceName ) x( static_cast< CSS::lang::XServiceName * >( this ) );
        return makeAny( x );
    }
    if (type.equals( ::getCppuType( (REF ( CSS::lang::XServiceInfo ) const *)0 ) )) {
        // return XServiceInfo interface
        REF ( CSS::lang::XServiceInfo ) x( static_cast< CSS::lang::XServiceInfo * >( this ) );
        return makeAny( x );
    }
    if (type.equals( ::getCppuType( (REF ( CSS::sheet::XAddIn ) const *)0 ) )) {
        // return XAddIn interface
        REF ( CSS::sheet::XAddIn ) x( static_cast< CSS::sheet::XAddIn * >( this ) );
        return makeAny( x );
    }
    if (type.equals( ::getCppuType( (REF ( CSS::sheet::addin::XQL ) const *)0 ) )) {
        // return sample interface
        REF ( CSS::sheet::addin::XQL ) x( static_cast< CSS::sheet::addin::XQL * >( this ) );
        return makeAny( x );
    }
    // querying for unsupported type
    return CSS::uno::Any();
}
void QLAddin::acquire() throw () {
    ::osl_incrementInterlockedCount( &m_refcount );
}
void QLAddin::release() throw () {
    if (0 == ::osl_decrementInterlockedCount( &m_refcount )) {
        delete this;
    }
}
QLAddin::QLAddin() throw () : m_refcount( 0 ) { 
//	qladdin
	funcMap[ STRFROMANSI( "incrementNum" ) ]
		=  STRFROMANSI( "QL_TEST1" );
//	options
	funcMap[ STRFROMANSI( "qlBlackScholes" ) ]
		=  STRFROMANSI( "QL_BLACKSCHOLES" );
	funcMap[ STRFROMANSI( "qlOption" ) ]
		=  STRFROMANSI( "QL_OPTION" );
	funcMap[ STRFROMANSI( "qlOptionSetEngine" ) ]
		=  STRFROMANSI( "QL_OPTION_SETENGINE" );
//	functions
	funcMap[ STRFROMANSI( "qlFieldNames" ) ]
		=  STRFROMANSI( "QL_FIELDNAMES" );
	funcMap[ STRFROMANSI( "qlValue" ) ]
		=  STRFROMANSI( "QL_VALUE" );
	funcMap[ STRFROMANSI( "qlLogfile" ) ]
		=  STRFROMANSI( "QL_LOGFILE" );
}

// XTypeProvider
SEQ ( CSS::uno::Type ) QLAddin::getTypes() THROWDEF_RTE
{
    SEQ ( CSS::uno::Type ) seq( 5 );
    seq[ 0 ] = ::getCppuType( (REF ( CSS::lang::XTypeProvider ) const *)0 );
    seq[ 1 ] = ::getCppuType( (REF ( CSS::lang::XServiceInfo ) const *)0 );
    seq[ 2 ] = ::getCppuType( (REF ( CSS::sheet::addin::XQL ) const *)0 );
    seq[ 3 ] = ::getCppuType( (REF ( CSS::lang::XServiceName ) const *)0 );
    seq[ 4 ] = ::getCppuType( (REF ( CSS::sheet::XAddIn ) const *)0 );
    return seq;
}
SEQ ( sal_Int8 ) QLAddin::getImplementationId() THROWDEF_RTE {
    static SEQ ( sal_Int8 ) * s_pId = 0;
    if (! s_pId)
    {
        // create unique id
        SEQ ( sal_Int8 ) id( 16 );
        ::rtl_createUuid( (sal_uInt8 *)id.getArray(), 0, sal_True );
        // guard initialization with some mutex
        ::osl::MutexGuard guard( ::osl::Mutex::getGlobalMutex() );
        if (! s_pId)
        {
            static SEQ ( sal_Int8 ) s_id( id );
            s_pId = &s_id;
        }
    }
    return *s_pId;
}

	// Static

STRING QLAddin::getImplementationName_Static()
{
	return STRFROMASCII( MY_IMPLNAME );
}

SEQ( STRING ) QLAddin::getSupportedServiceNames_Static()
{
	SEQ( STRING )	aRet(2);
	STRING*			pArray = aRet.getArray();
	pArray[0] = STRFROMASCII( ADDIN_SERVICE );
	pArray[1] = STRFROMASCII( MY_SERVICE );
	return aRet;
}

	// XAddIn
STRING SAL_CALL QLAddin::getProgrammaticFuntionName( const STRING& aDisplayName ) THROWDEF_RTE
{
	return STRING();
}

STRING SAL_CALL QLAddin::getDisplayFunctionName( const STRING& aProgrammaticName ) THROWDEF_RTE
{
	return funcMap[ aProgrammaticName ];
}

STRING SAL_CALL QLAddin::getFunctionDescription( const STRING& aProgrammaticName ) THROWDEF_RTE
{
	return STRFROMANSI( "This function accepts as input a single integer, the return value is one plus the input value" );
}

STRING SAL_CALL QLAddin::getDisplayArgumentName( const STRING& aName, sal_Int32 nArg ) THROWDEF_RTE
{
	return STRFROMANSI( "argument 1" );
}

STRING SAL_CALL QLAddin::getArgumentDescription( const STRING& aName, sal_Int32 nArg ) THROWDEF_RTE
{
	return STRFROMANSI( "Number to increment" );
}

STRING SAL_CALL QLAddin::getProgrammaticCategoryName(STRING const & str) THROWDEF_RTE
{
	return STRFROMASCII( "Add-In" );
}

STRING SAL_CALL QLAddin::getDisplayCategoryName(STRING const & str) THROWDEF_RTE
{
	return STRFROMASCII( "Add-In" );
}

	// XServiceName
STRING SAL_CALL	QLAddin::getServiceName(  ) THROWDEF_RTE
{
	return STRFROMASCII( MY_SERVICE );
}

	// XServiceInfo

STRING SAL_CALL	QLAddin::getImplementationName(  ) THROWDEF_RTE
{
	return getImplementationName_Static();
}

sal_Bool SAL_CALL QLAddin::supportsService( const STRING& aName ) THROWDEF_RTE
{
	return aName.compareToAscii( ADDIN_SERVICE ) == 0 || aName.compareToAscii( MY_SERVICE ) == 0;
}

SEQ( STRING ) SAL_CALL QLAddin::getSupportedServiceNames(  ) THROWDEF_RTE
{
	return getSupportedServiceNames_Static();
}

	// XLocalizable

void SAL_CALL QLAddin::setLocale( const lang::Locale& eLocale ) THROWDEF_RTE
{
	aFuncLoc = eLocale;
}

lang::Locale SAL_CALL QLAddin::getLocale() THROWDEF_RTE
{
	return aFuncLoc;
}

sal_Int32 SAL_CALL QLAddin::incrementNum( sal_Int32 nVal ) THROWDEF_RTE_IAE {
	return nVal + 1;
}
